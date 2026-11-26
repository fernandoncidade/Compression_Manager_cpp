#include "test_common.hpp"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QUrl>
#include <QXmlStreamReader>

namespace {

QString readTextUtf8(const QString& absPath) {
    QFile f(absPath);
    if (!f.open(QIODevice::ReadOnly)) return QString();
    QByteArray data = f.readAll();
    f.close();
    if (data.startsWith("\xEF\xBB\xBF")) data = data.mid(3);
    return QString::fromUtf8(data);
}

QString extractIssAppVersion(const QString& issText) {
    static const QRegularExpression re(
        QStringLiteral(R"cm(#define\s+MyAppVersion\s+"(\d+\.\d+\.\d+\.\d+)")cm"));
    const QRegularExpressionMatch m = re.match(issText);
    return m.hasMatch() ? m.captured(1) : QString();
}

} // namespace

class TestRepoIntegrityHygiene : public QObject {
    Q_OBJECT

private slots:
    void testAppxManifestXmlAndStoreMetadata() {
        const QString path = QDir(cm_test::sourceRootDir()).filePath("library/AppxManifest.xml");
        QVERIFY2(QFileInfo::exists(path), qPrintable("Missing AppxManifest.xml: " + path));

        const QString xmlText = readTextUtf8(path);
        QVERIFY(!xmlText.isEmpty());

        QXmlStreamReader xml(xmlText);
        bool sawIdentity = false;
        bool sawRunFullTrust = false;
        bool sawIntegrity = false;

        while (!xml.atEnd()) {
            xml.readNext();
            if (xml.isStartElement()) {
                const QString name = xml.name().toString();
                if (name == "Identity") {
                    sawIdentity = true;
                    QCOMPARE(xml.attributes().value("Name").toString(), QStringLiteral("fernandoncidade.COMPRESSIONMANAGER"));
                    QCOMPARE(xml.attributes().value("ProcessorArchitecture").toString(), QStringLiteral("x64"));
                } else if (name == "Capability" && xml.attributes().value("Name").toString() == "runFullTrust") {
                    sawRunFullTrust = true;
                } else if (name == "PackageIntegrity") {
                    sawIntegrity = true;
                }
            }
        }
        QVERIFY2(!xml.hasError(), qPrintable("XML error in AppxManifest.xml: " + xml.errorString()));
        QVERIFY(sawIdentity);
        QVERIFY(sawRunFullTrust);
        QVERIFY(sawIntegrity);
    }

    void testCMakePresetsAndVsCodeTasksIntegration() {
        const QString root = cm_test::sourceRootDir();

        // 1. CMakePresets.json
        {
            const QString presetsPath = QDir(root).filePath("CMakePresets.json");
            const QByteArray raw = readTextUtf8(presetsPath).toUtf8();
            QJsonParseError err{};
            const QJsonDocument doc = QJsonDocument::fromJson(raw, &err);
            QVERIFY2(err.error == QJsonParseError::NoError && doc.isObject(),
                     qPrintable("CMakePresets.json parse error: " + err.errorString()));

            const QJsonObject obj = doc.object();
            const QJsonArray cfgArr = obj.value("configurePresets").toArray();
            const QJsonArray testArr = obj.value("testPresets").toArray();

            QMap<QString, QJsonObject> cfgByName;
            for (const QJsonValue& v : cfgArr) {
                const QJsonObject o = v.toObject();
                cfgByName.insert(o.value("name").toString(), o);
            }

            for (const QString& req : {
                "full-mingw-release", "full-msvc-release",
                "standart-mingw-release", "standart-msvc-release",
                "mocks-mingw-release", "mocks-msvc-release",
                "suite-mingw-release", "suite-msvc-release"
            }) {
                QVERIFY2(cfgByName.contains(req), qPrintable("Missing configurePreset: " + req));
            }

            QCOMPARE(cfgByName["suite-msvc-release"].value("binaryDir").toString(),
                     QStringLiteral("${sourceDir}/build/build_suite_msvc"));
            QCOMPARE(cfgByName["suite-mingw-release"].value("binaryDir").toString(),
                     QStringLiteral("${sourceDir}/build/build_suite_mingw"));
            QCOMPARE(cfgByName["suite-msvc-release"].value("cacheVariables").toObject().value("BUILD_TESTS").toString(),
                     QStringLiteral("ON"));
            QCOMPARE(cfgByName["suite-mingw-release"].value("cacheVariables").toObject().value("BUILD_TESTS").toString(),
                     QStringLiteral("ON"));
            QCOMPARE(cfgByName["base-msvc-qt6-release"].value("cacheVariables").toObject().value("CMAKE_CXX_STANDARD").toString(),
                     QStringLiteral("23"));
            QCOMPARE(cfgByName["base-mingw-qt6-release"].value("cacheVariables").toObject().value("CMAKE_CXX_STANDARD").toString(),
                     QStringLiteral("23"));

            QSet<QString> testPresetNames;
            for (const QJsonValue& v : testArr) {
                testPresetNames.insert(v.toObject().value("name").toString());
            }
            QVERIFY(testPresetNames.contains("suite-msvc-release"));
            QVERIFY(testPresetNames.contains("suite-mingw-release"));
        }

        // 2. .vscode/tasks.json
        {
            const QString tasksPath = QDir(root).filePath(".vscode/tasks.json");
            const QByteArray raw = readTextUtf8(tasksPath).toUtf8();
            QJsonParseError err{};
            const QJsonDocument doc = QJsonDocument::fromJson(raw, &err);
            QVERIFY2(err.error == QJsonParseError::NoError && doc.isObject(),
                     qPrintable(".vscode/tasks.json parse error: " + err.errorString()));

            const QJsonArray tasks = doc.object().value("tasks").toArray();
            QMap<QString, QJsonObject> byLabel;
            QStringList visibleLabels;
            QMap<QString, QJsonObject> hiddenByLabel;

            for (const QJsonValue& v : tasks) {
                const QJsonObject t = v.toObject();
                const QString label = t.value("label").toString();
                byLabel.insert(label, t);
                if (t.value("hide").toBool(false)) {
                    QVERIFY2(!t.contains("group"),
                             qPrintable("Hidden subtask must not define 'group': " + label));
                    hiddenByLabel.insert(label, t);
                } else {
                    visibleLabels.append(label);
                }
            }

            const QStringList expectedVisibleOrder = {
                QStringLiteral("Qt6: Compilar e Executar CM-FULL (MinGW)"),
                QStringLiteral("Qt6: Compilar e Executar CM-FULL (MSVC)"),
                QStringLiteral("Qt6: Compilar e Executar CM-STANDART (MinGW)"),
                QStringLiteral("Qt6: Compilar e Executar CM-STANDART (MSVC)"),
                QStringLiteral("Qt6: Compilar e Executar MOCKS (MinGW)"),
                QStringLiteral("Qt6: Compilar e Executar MOCKS (MSVC)"),
                QStringLiteral("Qt6: Compilar e Executar SUITE (MinGW)"),
                QStringLiteral("Qt6: Compilar e Executar SUITE (MSVC)")
            };
            QCOMPARE(visibleLabels, expectedVisibleOrder);

            for (const QString& label : visibleLabels) {
                const QJsonArray deps = byLabel[label].value("dependsOn").toArray();
                for (const QJsonValue& depVal : deps) {
                    const QString depLabel = depVal.toString();
                    QVERIFY2(hiddenByLabel.contains(depLabel),
                             qPrintable("dependsOn target must exist and be hidden: " + depLabel));
                }
            }

            const QString msvcLabel = QStringLiteral("Qt6: Compilar e Executar SUITE (MSVC)");
            const QString mingwLabel = QStringLiteral("Qt6: Compilar e Executar SUITE (MinGW)");
            QVERIFY2(byLabel.contains(msvcLabel), "Missing required MSVC SUITE task label");
            QVERIFY2(byLabel.contains(mingwLabel), "Missing required MinGW SUITE task label");

            const QString msvcCmd = byLabel[msvcLabel].value("command").toString();
            const QString mingwCmd = byLabel[mingwLabel].value("command").toString();
            QVERIFY(msvcCmd.contains("suite-msvc-release") && msvcCmd.contains("ctest") && msvcCmd.contains("run_msvc.bat"));
            QVERIFY(mingwCmd.contains("suite-mingw-release") && mingwCmd.contains("ctest") && mingwCmd.contains("run_mingw.bat"));
        }
    }

    void testInnoSetupGuardrailsAndNoSuiteExternalDistribution() {
        const QString root = cm_test::sourceRootDir();

        QVERIFY2(QFileInfo::exists(QDir(root).filePath("library/Compression_Manager_cpp.txt")),
                 "Missing library/Compression_Manager_cpp.txt");

        QString unifiedIssVersion;
        for (const QString& issName : {
            "library/Compression_Manager.iss",
            "library/Compression_Manager_MSVC.iss",
            "library/Compression_Manager_MinGW.iss"
        }) {
            const QString txt = readTextUtf8(QDir(root).filePath(issName));
            QVERIFY2(!txt.isEmpty(), qPrintable("Missing or empty Inno script: " + issName));
            const QString appVer = extractIssAppVersion(txt);
            QVERIFY2(!appVer.isEmpty(), qPrintable("Missing #define MyAppVersion in: " + issName));
            if (unifiedIssVersion.isEmpty()) {
                unifiedIssVersion = appVer;
            } else {
                QCOMPARE(appVer, unifiedIssVersion);
            }
            QVERIFY(txt.contains("6.12.0.0"));
            QVERIFY(txt.contains("rarreg.key"));
            QVERIFY(txt.contains("Qt6Network.dll"));
            QVERIFY(txt.contains("vc_redist") || txt.contains("app-local"));
        }

        // Garante que gerar_distribuicoes_limpas.ps1 e tests/CMakeLists.txt NAO criam distribuicao externa para a suite
        const QString distScript = readTextUtf8(QDir(root).filePath("cmake/gerar_distribuicoes_limpas.ps1"));
        QVERIFY(!distScript.isEmpty());
        QVERIFY(!distScript.contains("build_suite_msvc"));
        QVERIFY(!distScript.contains("build_suite_mingw"));

        const QString suiteCmake = readTextUtf8(QDir(root).filePath("tests/CMakeLists.txt"));
        QVERIFY(!suiteCmake.isEmpty());
        QVERIFY(!suiteCmake.contains("gerar_distribuicoes_limpas"));
        QVERIFY(!suiteCmake.contains("MISCELANEAS"));
    }

    void testDocumentationAndDualLicensingCompleteness() {
        const QString root = cm_test::sourceRootDir();

        // Exige ausencia estrita de cli-reference.md e PSF license
        QVERIFY(!QFileInfo::exists(QDir(root).filePath("docs/cli-reference.md")));
        QVERIFY(!QFileInfo::exists(QDir(root).filePath("source/assets/LICENSES/PYTHON SOFTWARE FOUNDATION LICENSE.txt")));

        for (const QString& reqRootFile : {
            "LICENSE", "README.md", "MANUAL.md", "OBSERVACAO.md",
            "CODE_OF_CONDUCT.md", "CONTRIBUTING.md", "SECURITY.md",
            "docs/images/banner.png", "docs/images/logo.png",
            "source/assets/LICENSES/GNU AFFERO GENERAL PUBLIC LICENSE Version 3.txt",
            "source/assets/LICENSES/GNU GENERAL PUBLIC LICENSE Version 3.txt",
            "source/assets/RELEASE/RELEASE NOTES_pt_BR.txt",
            "source/assets/RELEASE/RELEASE NOTES_en_US.txt",
            "source/assets/RELEASE/RELEASE NOTES_Standart_pt_BR.txt",
            "source/assets/RELEASE/RELEASE NOTES_Standart_en_US.txt"
        }) {
            const QString p = QDir(root).filePath(reqRootFile);
            QVERIFY2(QFileInfo::exists(p) && QFileInfo(p).size() > 0,
                     qPrintable("Missing required repo documentation/license asset: " + reqRootFile));
        }

        for (const QString& stdRelFile : {
            "source/assets/RELEASE/RELEASE NOTES_Standart_pt_BR.txt",
            "source/assets/RELEASE/RELEASE NOTES_Standart_en_US.txt"
        }) {
            const QString stdText = readTextUtf8(QDir(root).filePath(stdRelFile));
            QVERIFY2(!stdText.contains("WinRAR"),
                     qPrintable("Forbidden WinRAR reference in Standart release notes: " + stdRelFile));
            QVERIFY2(!stdText.contains(".rar", Qt::CaseInsensitive),
                     qPrintable("Forbidden .rar reference in Standart release notes: " + stdRelFile));
        }

        const QString licenseText = readTextUtf8(QDir(root).filePath("LICENSE"));
        QVERIFY(licenseText.contains("GNU AFFERO GENERAL PUBLIC LICENSE") && licenseText.contains("Version 3"));

        const QString mainIssVersion = extractIssAppVersion(
            readTextUtf8(QDir(root).filePath("library/Compression_Manager.iss")));
        QVERIFY2(!mainIssVersion.isEmpty(), "Failed to extract MyAppVersion from library/Compression_Manager.iss");

        const QString releaseDocRel = QStringLiteral("docs/release-v%1.md").arg(mainIssVersion);
        const QStringList requiredDocs = {
            QStringLiteral("docs/architecture.md"),
            QStringLiteral("docs/compliance.md"),
            QStringLiteral("docs/licensing.md"),
            QStringLiteral("docs/privacy-policy.md"),
            releaseDocRel,
            QStringLiteral("docs/security-model.md"),
            QStringLiteral("docs/suggested-issues.md"),
            QStringLiteral("docs/technical-audit-and-roadmap.md"),
            QStringLiteral("docs/user-guide.md")
        };

        for (const QString& docFile : requiredDocs) {
            const QString p = QDir(root).filePath(docFile);
            QVERIFY2(QFileInfo::exists(p) && QFileInfo(p).size() > 200,
                     qPrintable("Missing or too small doc: " + docFile));
        }

        const QString readmeText = readTextUtf8(QDir(root).filePath("README.md"));
        for (const QString& locale : {QStringLiteral("pt_BR"), QStringLiteral("en_US")}) {
            const QDir ifaceDir(QDir(root).filePath("images/interfaces/" + locale));
            const QFileInfoList imgs = ifaceDir.entryInfoList(
                QStringList() << "*.png" << "*.jpg" << "*.jpeg" << "*.webp", QDir::Files);
            QVERIFY2(!imgs.isEmpty(), qPrintable("Missing interface screenshots in: " + ifaceDir.path()));
            for (const QFileInfo& fi : imgs) {
                const QString relPath = QStringLiteral("images/interfaces/%1/%2").arg(locale, fi.fileName());
                const QString encodedPath = QString::fromUtf8(QUrl::toPercentEncoding(relPath, "/"));
                QVERIFY2(readmeText.contains(relPath) || readmeText.contains(encodedPath),
                         qPrintable("README.md does not report interface image: " + relPath));
            }
        }
    }
};

CM_TEST_MAIN(TestRepoIntegrityHygiene)
#ifndef __INTELLISENSE__
#include "test_10_repo_integrity_hygiene.moc"
#endif
