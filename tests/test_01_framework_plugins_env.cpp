#include "test_common.hpp"

#include <QDir>
#include <QFileInfo>
#include <QIcon>
#include <QImage>
#include <QImageReader>
#include <QLibraryInfo>
#include <QPluginLoader>
#include <QSettings>
#include <QStyleFactory>
#include <QtGlobal>

class TestFrameworkPluginsEnv : public QObject {
    Q_OBJECT

private slots:
    void testCpp23StandardAndQtVersion() {
#if defined(_MSVC_LANG)
        QVERIFY2(_MSVC_LANG >= 202302L,
                 qPrintable(QString("MSVC C++ standard too low: %1").arg(_MSVC_LANG)));
#else
        QVERIFY2(__cplusplus >= 202302L,
                 qPrintable(QString("GCC/MinGW C++ standard too low: %1").arg(__cplusplus)));
#endif

        QCOMPARE(QT_VERSION_MAJOR, 6);
        QCOMPARE(QT_VERSION_MINOR, 12);
        QCOMPARE(QString::fromLatin1(qVersion()), QString::fromUtf8(CM_QT_VERSION_EXPECTED));
        QCOMPARE(QLibraryInfo::version().majorVersion(), 6);
        QCOMPARE(QLibraryInfo::version().minorVersion(), 12);
    }

    void testQtConfAnchoringAndPlugins() {
        const QString appDir = QCoreApplication::applicationDirPath();
        const QString qtConfPath = QDir(appDir).filePath("qt.conf");
        QVERIFY2(QFileInfo::exists(qtConfPath), qPrintable("Missing qt.conf at: " + qtConfPath));

        QSettings qtConf(qtConfPath, QSettings::IniFormat);
        QCOMPARE(qtConf.value("Paths/Prefix").toString(), QStringLiteral("."));
        QCOMPARE(qtConf.value("Paths/Plugins").toString(), QStringLiteral("plugins"));
        QCOMPARE(qtConf.value("Paths/Translations").toString(), QStringLiteral("translations"));

        const QString platform = QGuiApplication::platformName();
        QVERIFY2(platform == "offscreen" || platform == "windows",
                 qPrintable("Unexpected active QPA platform: " + platform));

        const QStringList requiredPlugins = {
            "plugins/platforms/qwindows.dll",
            "plugins/platforms/qoffscreen.dll",
            "plugins/styles/qmodernwindowsstyle.dll",
            "plugins/imageformats/qico.dll"
        };
        for (const QString& rel : requiredPlugins) {
            const QString full = QDir(appDir).filePath(rel);
            QVERIFY2(QFileInfo::exists(full) && QFileInfo(full).size() > 0,
                     qPrintable("Missing or empty Qt plugin: " + full));
        }

        const QStringList styles = QStyleFactory::keys();
        QVERIFY(!styles.isEmpty());
        bool hasModernOrFusion = false;
        for (const QString& s : styles) {
            if (s.compare("windows11", Qt::CaseInsensitive) == 0 ||
                s.compare("windowsvista", Qt::CaseInsensitive) == 0 ||
                s.compare("Fusion", Qt::CaseInsensitive) == 0) {
                hasModernOrFusion = true;
                break;
            }
        }
        QVERIFY2(hasModernOrFusion, qPrintable("Available styles: " + styles.join(", ")));

        const QList<QByteArray> formats = QImageReader::supportedImageFormats();
        QVERIFY2(formats.contains("png"), "PNG image plugin/support unavailable");
        QVERIFY2(formats.contains("ico"), "ICO imageformat plugin (qico.dll) failed to load");
    }

    void testBundledIconsDecodeProperly() {
        const QString appDir = QCoreApplication::applicationDirPath();
        const QString icoPath = QDir(appDir).filePath("assets/icones/Manager-BackUp.ico");
        const QString pngPath = QDir(appDir).filePath("assets/icones/pasta.png");

        QVERIFY2(QFileInfo::exists(icoPath), qPrintable("Missing ICO: " + icoPath));
        QVERIFY2(QFileInfo::exists(pngPath), qPrintable("Missing PNG: " + pngPath));

        QIcon appIcon(icoPath);
        QVERIFY(!appIcon.isNull());
        QVERIFY(!appIcon.pixmap(32, 32).isNull());

        QImage pngImg(pngPath);
        QVERIFY(!pngImg.isNull());
        QVERIFY(pngImg.width() > 0 && pngImg.height() > 0);
    }

    void testToolchainRuntimeIsolationAndNoBloat() {
        const QString appDir = QCoreApplication::applicationDirPath();
        const QDir dir(appDir);

        // Nenhum instalador vc_redist*.exe pode estar empacotado
        const QStringList redistMatches = dir.entryList(
            { "vc_redist*.exe", "vcredist*.exe", "VC_redist*.exe" },
            QDir::Files
        );
        QVERIFY2(redistMatches.isEmpty(), qPrintable("Forbidden redist installer found: " + redistMatches.join(", ")));

        // Nenhuma DLL Qt nao utilizada
        for (const QString& unusedQtDll : { "Qt6Network.dll", "Qt6Pdf.dll", "Qt6Svg.dll", "Qt6PrintSupport.dll" }) {
            QVERIFY2(!QFileInfo::exists(dir.filePath(unusedQtDll)),
                     qPrintable("Unused Qt DLL leaked into suite dir: " + unusedQtDll));
        }

        const QString tc = cm_test::toolchainName();
        const QStringList msvcDlls = {
            "msvcp140.dll", "msvcp140_1.dll", "msvcp140_2.dll",
            "vcruntime140.dll", "vcruntime140_1.dll"
        };
        const QStringList mingwDlls = {
            "libgcc_s_seh-1.dll", "libstdc++-6.dll", "libwinpthread-1.dll"
        };

        if (tc == "MSVC") {
            for (const QString& dll : msvcDlls) {
                QVERIFY2(QFileInfo::exists(dir.filePath(dll)), qPrintable("Missing MSVC CRT DLL: " + dll));
            }
            for (const QString& dll : mingwDlls) {
                QVERIFY2(!QFileInfo::exists(dir.filePath(dll)), qPrintable("MinGW DLL leaked into MSVC suite: " + dll));
            }
        } else if (tc == "MinGW") {
            for (const QString& dll : mingwDlls) {
                QVERIFY2(QFileInfo::exists(dir.filePath(dll)), qPrintable("Missing MinGW GCC 15.1 DLL: " + dll));
            }
            for (const QString& dll : msvcDlls) {
                QVERIFY2(!QFileInfo::exists(dir.filePath(dll)), qPrintable("MSVC DLL leaked into MinGW suite: " + dll));
            }
        } else {
            QFAIL(qPrintable("Unknown toolchain macro: " + tc));
        }
    }
};

CM_TEST_MAIN(TestFrameworkPluginsEnv)
#ifndef __INTELLISENSE__
#include "test_01_framework_plugins_env.moc"
#endif
