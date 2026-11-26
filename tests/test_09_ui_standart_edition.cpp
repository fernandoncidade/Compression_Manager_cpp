#include "test_common.hpp"

#include "source/GerenciamentoUI/ui_06_SobreDialog.hpp"
#include "source/GerenciamentoUI/ui_07_Manual.hpp"
#include "source/GerenciamentoUI/ui_08_Release_en_US.hpp"
#include "source/GerenciamentoUI/ui_09_Lancamento_pt_BR.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/MotoresCompressao/mtcomp_02_motoresCompressao.hpp"
#include "source/utils/ApplicationPathUtils.hpp"
#include "source/utils/CaminhoPersistenteUtils.hpp"

#include <QCheckBox>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLabel>
#include <QTextBrowser>

class TestUiStandartEdition : public QObject {
    Q_OBJECT

private slots:
    void init() {
        const QString persistDir = obter_caminho_persistente();
        QFile::remove(QDir(persistDir).filePath("language.json"));
        QFile::remove(QDir(persistDir).filePath("config.json"));
    }

    void testCompileTimeSegregationOfRarSupport() {
#if ENABLE_RAR_SUPPORT
        QFAIL("test_09_ui_standart_edition MUST be compiled with ENABLE_RAR_SUPPORT=0");
#endif
        QVERIFY2(buscar_winrar_executavel().isEmpty(),
                 "buscar_winrar_executavel() MUST return empty string in Standart edition");

        InterfaceGrafica w;
        w.show();
        QCoreApplication::processEvents();

        // Todas as acoes de empacotamento RAR precisam ser nulas na edicao Standart
        QVERIFY(w.especificar_rar_action == nullptr);
        QVERIFY(w.armazenar_rar_action == nullptr);
        QVERIFY(w.limpar_saida_rar_action == nullptr);

        // Exatamente 5 modos abertos: zip, 7z, tar, wim, extracao (sem rar!)
        QCOMPARE(w.compression_method_layouts.size(), 5);
        QCOMPARE(w.checkboxes.size(), 5);
        QVERIFY(!w.compression_method_layouts.contains("rar"));
        QVERIFY(!w.checkboxes.contains("rar"));
        for (const QString& key : { "zip", "7z", "tar", "wim", "extracao" }) {
            QVERIFY(w.checkboxes.contains(key));
        }

        // Submenus de metodos nao podem conter RAR
        QVERIFY(!w.method_actions.contains("rar"));
        QVERIFY(w.method_actions.contains("zip"));
        QVERIFY(w.method_actions.contains("7z"));
        QVERIFY(w.method_actions.contains("tar"));
        QVERIFY(w.method_actions.contains("wim"));

        // Layout Duplo em Standart tambem exclui RAR
        w.segundo_layout_action->trigger();
        QCoreApplication::processEvents();
        QCOMPARE(w.checkboxes_2.size(), 5);
        QVERIFY(!w.checkboxes_2.contains("rar"));

        // Alternar checkboxes e trocar idioma funciona normalmente nos modos Standart
        w.checkboxes["zip"]->setChecked(true);
        w.checkboxes_2["7z"]->setChecked(true);
        QCoreApplication::processEvents();
        QCOMPARE(w.current_layouts.size(), 1);
        QCOMPARE(w.current_layouts_2.size(), 1);

        w.mudar_idioma("en_US");
        QCoreApplication::processEvents();
        QCOMPARE(w.arquivo_menu->title(), QStringLiteral("File"));
        QCOMPARE(w.checkboxes["extracao"]->text(), QStringLiteral("EXTRACTION"));

        // ManualDialog e SobreDialog devem identificar Edicao Standart
        w.mudar_idioma("pt_BR");
        QCoreApplication::processEvents();

        w.manual_action->trigger();
        w.sobre_action->trigger();
        QCoreApplication::processEvents();
        QVERIFY(!w.dialog_manual.isNull());
        QVERIFY(!w.dialog_sobre.isNull());
        QVERIFY(w.dialog_manual->windowTitle().contains("Standart"));

        auto* manualBrowser = w.dialog_manual->findChild<QTextBrowser*>();
        QVERIFY(manualBrowser != nullptr);
        QVERIFY(manualBrowser->toPlainText().contains("Standart"));

        auto* sobreLabel = w.dialog_sobre->findChild<QLabel*>();
        QVERIFY(sobreLabel != nullptr);
        QVERIFY(!sobreLabel->text().contains("(RAR,", Qt::CaseInsensitive));

        const QString runtimeAppDir = QDir::cleanPath(QCoreApplication::applicationDirPath());
        const QString stdPtPath = get_text_file_path("RELEASE NOTES_Standart_pt_BR.txt", "RELEASE");
        const QString stdEnPath = get_text_file_path("RELEASE NOTES_Standart_en_US.txt", "RELEASE");
        QVERIFY2(QFileInfo::exists(stdPtPath) && QFileInfo(stdPtPath).isFile(),
                 qPrintable("Missing runtime Standart release notes (pt_BR): " + stdPtPath));
        QVERIFY2(QFileInfo::exists(stdEnPath) && QFileInfo(stdEnPath).isFile(),
                 qPrintable("Missing runtime Standart release notes (en_US): " + stdEnPath));
        QVERIFY2(QDir::cleanPath(stdPtPath).startsWith(runtimeAppDir, Qt::CaseInsensitive),
                 qPrintable("Standart release notes (pt_BR) resolved outside runtime app dir: " + stdPtPath));
        QVERIFY2(QDir::cleanPath(stdEnPath).startsWith(runtimeAppDir, Qt::CaseInsensitive),
                 qPrintable("Standart release notes (en_US) resolved outside runtime app dir: " + stdEnPath));

        const QString relPt = LancamentoPtBR::obter_notas_lancamento_pt_BR();
        const QString relEn = ReleaseNotesEnUS::obter_release_notes_en_US();
        QVERIFY(relPt.contains("2026.11.22.0") && relPt.contains("2026.10.17.0") && relPt.contains("formatos abertos"));
        QVERIFY(relEn.contains("2026.11.22.0") && relEn.contains("2026.10.17.0") && relEn.contains("open formats"));
        QVERIFY(!relPt.contains("WinRAR"));
        QVERIFY(!relEn.contains("WinRAR"));
        QVERIFY(!relPt.contains(".rar", Qt::CaseInsensitive));
        QVERIFY(!relEn.contains(".rar", Qt::CaseInsensitive));

        w.close();
        QCoreApplication::processEvents();
    }
};

CM_TEST_MAIN(TestUiStandartEdition)
#ifndef __INTELLISENSE__
#include "test_09_ui_standart_edition.moc"
#endif
