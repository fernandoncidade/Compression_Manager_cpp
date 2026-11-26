#include "test_common.hpp"

#include "source/GerenciamentoUI/ui_05_OpcoesSobre.hpp"
#include "source/GerenciamentoUI/ui_06_SobreDialog.hpp"
#include "source/GerenciamentoUI/ui_07_Manual.hpp"
#include "source/GerenciamentoUI/ui_08_Release_en_US.hpp"
#include "source/GerenciamentoUI/ui_09_Lancamento_pt_BR.hpp"
#include "source/utils/ApplicationPathUtils.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLabel>
#include <QPushButton>
#include <QRegularExpression>
#include <QTabWidget>
#include <QTextBrowser>

class TestAssetsLegalLicensing : public QObject {
    Q_OBJECT

private slots:
    void testLegalDocumentsDualLicensingAndStackSync() {
        OpcoesSobre::carregar_textos_sobre();

        const QList<QPair<QString, QString>> fields = {
            { "SITE_LICENSES", OpcoesSobre::SITE_LICENSES },
            { "LICENSE_OWNER", OpcoesSobre::LICENSE_OWNER },
            { "LICENSE_TEXT_PT_BR", OpcoesSobre::LICENSE_TEXT_PT_BR },
            { "LICENSE_TEXT_EN_US", OpcoesSobre::LICENSE_TEXT_EN_US },
            { "NOTICE_TEXT_PT_BR", OpcoesSobre::NOTICE_TEXT_PT_BR },
            { "NOTICE_TEXT_EN_US", OpcoesSobre::NOTICE_TEXT_EN_US },
            { "ABOUT_TEXT_PT_BR", OpcoesSobre::ABOUT_TEXT_PT_BR },
            { "ABOUT_TEXT_EN_US", OpcoesSobre::ABOUT_TEXT_EN_US },
            { "Privacy_Policy_pt_BR", OpcoesSobre::Privacy_Policy_pt_BR },
            { "Privacy_Policy_en_US", OpcoesSobre::Privacy_Policy_en_US },
            { "History_APP_pt_BR", OpcoesSobre::History_APP_pt_BR },
            { "History_APP_en_US", OpcoesSobre::History_APP_en_US },
            { "RELEASE_NOTES_pt_BR", OpcoesSobre::RELEASE_NOTES_pt_BR },
            { "RELEASE_NOTES_en_US", OpcoesSobre::RELEASE_NOTES_en_US },
        };

        for (const auto& f : fields) {
            QVERIFY2(!f.second.trimmed().isEmpty(), qPrintable("Empty asset text field: " + f.first));
        }

        // Verifica duplo licenciamento (GNU AGPLv3 + EULA/CLC), Qt 6.12.0 e ausencia de stack Python legada
        QVERIFY(OpcoesSobre::LICENSE_OWNER.contains("AGPL", Qt::CaseInsensitive) ||
                OpcoesSobre::LICENSE_OWNER.contains("GNU AFFERO", Qt::CaseInsensitive));
        QVERIFY(OpcoesSobre::LICENSE_TEXT_PT_BR.contains("EULA", Qt::CaseInsensitive) ||
                OpcoesSobre::LICENSE_TEXT_PT_BR.contains("AGPL", Qt::CaseInsensitive));
        QVERIFY(OpcoesSobre::LICENSE_TEXT_EN_US.contains("EULA", Qt::CaseInsensitive) ||
                OpcoesSobre::LICENSE_TEXT_EN_US.contains("AGPL", Qt::CaseInsensitive));

        static const QRegularExpression calverRx(QStringLiteral(R"re(\b\d{4}\.\d{1,2}\.\d{1,2}\.\d+\b)re"));
        const QRegularExpressionMatch aboutVerPt = calverRx.match(OpcoesSobre::ABOUT_TEXT_PT_BR);
        const QRegularExpressionMatch aboutVerEn = calverRx.match(OpcoesSobre::ABOUT_TEXT_EN_US);
        QVERIFY2(aboutVerPt.hasMatch(), "ABOUT_TEXT_PT_BR missing CalVer version string");
        QVERIFY2(aboutVerEn.hasMatch(), "ABOUT_TEXT_EN_US missing CalVer version string");
        QCOMPARE(aboutVerPt.captured(0), aboutVerEn.captured(0));

        QVERIFY(OpcoesSobre::ABOUT_TEXT_PT_BR.contains("Qt 6.12", Qt::CaseInsensitive));
        QVERIFY(OpcoesSobre::ABOUT_TEXT_EN_US.contains("Qt 6.12", Qt::CaseInsensitive));
        QVERIFY(OpcoesSobre::ABOUT_TEXT_PT_BR.contains("sem dependência de Python", Qt::CaseInsensitive));
        QVERIFY(OpcoesSobre::ABOUT_TEXT_EN_US.contains("zero Python", Qt::CaseInsensitive));
        QVERIFY(OpcoesSobre::NOTICE_TEXT_PT_BR.contains("7-Zip", Qt::CaseInsensitive));
        QVERIFY(OpcoesSobre::NOTICE_TEXT_EN_US.contains("7-Zip", Qt::CaseInsensitive));

        const QStringList forbiddenLegacyTokens = {
            "PySide6", "PyInstaller", "requirements.txt", "Python Software Foundation"
        };
        for (const auto& f : fields) {
            if (f.first == "SITE_LICENSES" ||
                f.first == "ABOUT_TEXT_PT_BR" ||
                f.first == "ABOUT_TEXT_EN_US" ||
                f.first == "History_APP_pt_BR" ||
                f.first == "History_APP_en_US" ||
                f.first == "RELEASE_NOTES_pt_BR" ||
                f.first == "RELEASE_NOTES_en_US") {
                continue;
            }
            for (const QString& legacy : forbiddenLegacyTokens) {
                QVERIFY2(!f.second.contains(legacy, Qt::CaseInsensitive),
                         qPrintable(QString("Obsolete Python token '%1' present in asset '%2'").arg(legacy, f.first)));
            }
        }
    }

    void testSobreDialogTabsLabelsAndFallback() {
        OpcoesSobre::carregar_textos_sobre();

        SobreDialog dlg(
            nullptr,
            "Sobre - COMPRESSION MANAGER",
            "<p>Cabeçalho</p>",
            OpcoesSobre::History_APP_pt_BR,
            OpcoesSobre::ABOUT_TEXT_PT_BR,
            OpcoesSobre::LICENSE_TEXT_PT_BR,
            OpcoesSobre::SITE_LICENSES,
            "Histórico", "Ocultar histórico",
            "Detalhes", "Ocultar detalhes",
            "Licenças", "Ocultar licenças",
            "OK",
            "Site oficial",
            OpcoesSobre::NOTICE_TEXT_PT_BR, "Avisos", "Ocultar avisos",
            OpcoesSobre::Privacy_Policy_pt_BR, "Política de Privacidade", "Ocultar política de privacidade",
            "Informação não disponível",
            OpcoesSobre::RELEASE_NOTES_pt_BR, "Notas de Versão", "Ocultar notas de versão"
        );

        auto* tabs = dlg.findChild<QTabWidget*>();
        QVERIFY(tabs != nullptr);
        QCOMPARE(tabs->count(), 6);

        // Aba inicial 0 deve exibir rotulo "Ocultar histórico", enquanto as demais exibem "Exibir"
        QCOMPARE(tabs->tabText(0), QStringLiteral("Ocultar histórico"));
        QCOMPARE(tabs->tabText(1), QStringLiteral("Detalhes"));
        QCOMPARE(tabs->tabText(2), QStringLiteral("Licenças"));

        tabs->setCurrentIndex(2);
        QCOMPARE(tabs->tabText(0), QStringLiteral("Histórico"));
        QCOMPARE(tabs->tabText(2), QStringLiteral("Ocultar licenças"));

        // Atualiza com texto vazio em avisos e privacidade para validar fallback
        dlg.atualizar_conteudo(
            "About - COMPRESSION MANAGER",
            "<p>Header</p>",
            "History EN",
            "Details EN",
            "Licenses EN",
            "https://example.org",
            "History", "Hide History",
            "Details", "Hide Details",
            "Licenses", "Hide Licenses",
            "OK",
            "Official Site",
            QString(), "Notices", "Hide Notices",
            QString(), "Privacy Policy", "Hide Privacy",
            "Information not available",
            "Release Notes EN", "Release Notes", "Hide Release Notes"
        );

        QCOMPARE(dlg.windowTitle(), QStringLiteral("About - COMPRESSION MANAGER"));
        auto* avisosTab = qobject_cast<QTextBrowser*>(tabs->widget(3));
        auto* privacidadeTab = qobject_cast<QTextBrowser*>(tabs->widget(4));
        QVERIFY(avisosTab != nullptr);
        QVERIFY(privacidadeTab != nullptr);
        QVERIFY(avisosTab->toPlainText().contains("Information not available"));
        QVERIFY(privacidadeTab->toPlainText().contains("Information not available"));
    }

    void testManualAndReleaseNotesContent() {
        ManualDialog manual(nullptr, "pt_BR");
        auto* browser = manual.findChild<QTextBrowser*>();
        QVERIFY(browser != nullptr);

        QVERIFY(manual.windowTitle().contains("Full"));
        const QString htmlPt = browser->toHtml();
        QVERIFY(htmlPt.contains("ZIP") && htmlPt.contains("7Z") && htmlPt.contains("TAR") && htmlPt.contains("WIM"));
        QVERIFY(htmlPt.contains("RAR"));

        manual.atualizar_traducao("en_US");
        QVERIFY(manual.windowTitle().contains("User Manual (Full Edition)"));
        const QString htmlEn = browser->toHtml();
        QVERIFY(htmlEn.contains("Archive") || htmlEn.contains("Extracting") || htmlEn.contains("Full Edition"));

        OpcoesSobre::carregar_textos_sobre();

        const QString runtimeAppDir = QDir::cleanPath(QCoreApplication::applicationDirPath());
        const QString ptPath = get_text_file_path("RELEASE NOTES_pt_BR.txt", "RELEASE");
        const QString enPath = get_text_file_path("RELEASE NOTES_en_US.txt", "RELEASE");

        QVERIFY2(QFileInfo::exists(ptPath) && QFileInfo(ptPath).isFile(),
                 qPrintable("Missing runtime release notes (pt_BR): " + ptPath));
        QVERIFY2(QFileInfo::exists(enPath) && QFileInfo(enPath).isFile(),
                 qPrintable("Missing runtime release notes (en_US): " + enPath));
        QVERIFY2(QDir::cleanPath(ptPath).startsWith(runtimeAppDir, Qt::CaseInsensitive),
                 qPrintable("Release notes (pt_BR) resolved outside runtime app dir: " + ptPath));
        QVERIFY2(QDir::cleanPath(enPath).startsWith(runtimeAppDir, Qt::CaseInsensitive),
                 qPrintable("Release notes (en_US) resolved outside runtime app dir: " + enPath));

        const QString relPt = LancamentoPtBR::obter_notas_lancamento_pt_BR();
        const QString relEn = ReleaseNotesEnUS::obter_release_notes_en_US();
        QVERIFY(relPt.contains("2026.11.22.0"));
        QVERIFY(relPt.contains("2026.10.17.0"));
        QVERIFY(relPt.contains("__________________________________________________________"));
        QVERIFY(relEn.contains("2026.11.22.0"));
        QVERIFY(relEn.contains("2026.10.17.0"));
        QVERIFY(relEn.contains("__________________________________________________________"));
        QCOMPARE(OpcoesSobre::RELEASE_NOTES_pt_BR, relPt);
        QCOMPARE(OpcoesSobre::RELEASE_NOTES_en_US, relEn);

        // Valida carregamento verdadeiramente dinâmico em disco (sem reiniciar o processo)
        struct RuntimeReleaseFileGuard {
            QString path;
            QByteArray originalBytes;
            bool active = false;
            ~RuntimeReleaseFileGuard() {
                if (active) {
                    cm_test::writeBytes(path, originalBytes);
                }
            }
        };

        auto readRaw = [](const QString& path, QByteArray* out) -> bool {
            QFile f(path);
            if (!f.open(QIODevice::ReadOnly)) return false;
            *out = f.readAll();
            f.close();
            return true;
        };

        RuntimeReleaseFileGuard ptGuard;
        QVERIFY(readRaw(ptPath, &ptGuard.originalBytes));
        ptGuard.path = ptPath;
        ptGuard.active = true;

        RuntimeReleaseFileGuard enGuard;
        QVERIFY(readRaw(enPath, &enGuard.originalBytes));
        enGuard.path = enPath;
        enGuard.active = true;

        const QByteArray ptToken = QByteArray("\n[DYNAMIC_RUNTIME_PROBE_PT_BR]\n");
        const QByteArray enToken = QByteArray("\n[DYNAMIC_RUNTIME_PROBE_EN_US]\n");
        QVERIFY(cm_test::writeBytes(ptPath, ptGuard.originalBytes + ptToken));
        QVERIFY(cm_test::writeBytes(enPath, enGuard.originalBytes + enToken));

        OpcoesSobre::carregar_textos_sobre();
        const QString relPtUpdated = LancamentoPtBR::obter_notas_lancamento_pt_BR();
        const QString relEnUpdated = ReleaseNotesEnUS::obter_release_notes_en_US();

        QVERIFY2(relPtUpdated.contains("[DYNAMIC_RUNTIME_PROBE_PT_BR]"),
                 "LancamentoPtBR did not reload updated RELEASE NOTES_pt_BR.txt dynamically");
        QVERIFY2(OpcoesSobre::RELEASE_NOTES_pt_BR.contains("[DYNAMIC_RUNTIME_PROBE_PT_BR]"),
                 "OpcoesSobre::RELEASE_NOTES_pt_BR stayed cached instead of reloading dynamically");
        QVERIFY2(relEnUpdated.contains("[DYNAMIC_RUNTIME_PROBE_EN_US]"),
                 "ReleaseNotesEnUS did not reload updated RELEASE NOTES_en_US.txt dynamically");
        QVERIFY2(OpcoesSobre::RELEASE_NOTES_en_US.contains("[DYNAMIC_RUNTIME_PROBE_EN_US]"),
                 "OpcoesSobre::RELEASE_NOTES_en_US stayed cached instead of reloading dynamically");

        QVERIFY(cm_test::writeBytes(ptPath, ptGuard.originalBytes));
        ptGuard.active = false;
        QVERIFY(cm_test::writeBytes(enPath, enGuard.originalBytes));
        enGuard.active = false;
        OpcoesSobre::carregar_textos_sobre();
        QCOMPARE(OpcoesSobre::RELEASE_NOTES_pt_BR, relPt);
        QCOMPARE(OpcoesSobre::RELEASE_NOTES_en_US, relEn);
    }
};

CM_TEST_MAIN(TestAssetsLegalLicensing)
#ifndef __INTELLISENSE__
#include "test_04_assets_legal_licensing.moc"
#endif
