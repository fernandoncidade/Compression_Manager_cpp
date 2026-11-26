#include "test_common.hpp"

#include "source/GerenciamentoUI/ui_06_SobreDialog.hpp"
#include "source/GerenciamentoUI/ui_07_Manual.hpp"
#include "source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_20_exibir_sobre.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/MotoresCompressao/mtcomp_01_metodosCompressao.hpp"
#include "source/utils/CaminhoPersistenteUtils.hpp"

#include <QBoxLayout>
#include <QCheckBox>
#include <QCloseEvent>
#include <QDir>
#include <QFile>
#include <QHBoxLayout>
#include <QVBoxLayout>

class TestUiFullEdition : public QObject {
    Q_OBJECT

private slots:
    void init() {
        const QString persistDir = obter_caminho_persistente();
        QFile::remove(QDir(persistDir).filePath("language.json"));
        QFile::remove(QDir(persistDir).filePath("config.json"));
    }

    void testFullEditionMenusCheckboxesAndScrollAdjustment() {
#if !ENABLE_RAR_SUPPORT
        QFAIL("test_08_ui_full_edition MUST be compiled with ENABLE_RAR_SUPPORT=1");
#endif
        InterfaceGrafica w;
        w.show();
        QCoreApplication::processEvents();

        // Verifica menus e acoes RAR exclusivos da edicao Full
        QVERIFY(w.arquivo_menu != nullptr);
        QVERIFY(w.especificar_saida_menu != nullptr);
        QVERIFY(w.armazenar_menu != nullptr);
        QVERIFY(w.limpar_saida_menu != nullptr);
        QVERIFY(w.layouts_menu != nullptr);
        QVERIFY(w.idiomas_menu != nullptr);
        QVERIFY(w.opcoes_menu != nullptr);

        QVERIFY(w.especificar_rar_action != nullptr);
        QVERIFY(w.armazenar_rar_action != nullptr);
        QVERIFY(w.limpar_saida_rar_action != nullptr);
        QVERIFY(w.method_actions.contains("rar"));

        // 6 modos na edicao Full: rar, zip, 7z, tar, wim, extracao
        QCOMPARE(w.compression_method_layouts.size(), 6);
        QCOMPARE(w.checkboxes.size(), 6);
        for (const QString& key : { "rar", "zip", "7z", "tar", "wim", "extracao" }) {
            QVERIFY2(w.checkboxes.contains(key) && w.checkboxes[key] != nullptr,
                     qPrintable("Missing checkbox in Full edition: " + key));
            QVERIFY(w.compression_method_layouts.contains(key));
        }

        // Politica de rolagem dinamica em ginter_17_adjust_scroll_area.cpp
        QCOMPARE(w.current_layouts.size(), 0);
        QCOMPARE(w.scroll_area->verticalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);

        w.checkboxes["rar"]->setChecked(true);
        QCoreApplication::processEvents();
        QCOMPARE(w.current_layouts.size(), 1);
        QVERIFY(w.current_layouts.contains("rar"));
        QCOMPARE(w.scroll_area->verticalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);

        // Ativar segundo modo habilita rolagem vertical automaticamente
        w.checkboxes["zip"]->setChecked(true);
        QCoreApplication::processEvents();
        QCOMPARE(w.current_layouts.size(), 2);
        QCOMPARE(w.scroll_area->verticalScrollBarPolicy(), Qt::ScrollBarAsNeeded);

        // Ativa todos os 6 modos simultaneamente e depois desativa todos
        for (auto it = w.checkboxes.begin(); it != w.checkboxes.end(); ++it) {
            it.value()->setChecked(true);
        }
        QCoreApplication::processEvents();
        QCOMPARE(w.current_layouts.size(), 6);

        for (auto it = w.checkboxes.begin(); it != w.checkboxes.end(); ++it) {
            it.value()->setChecked(false);
        }
        QCoreApplication::processEvents();
        QCOMPARE(w.current_layouts.size(), 0);
        QCOMPARE(w.scroll_area->verticalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);

        w.close();
    }

    void testDualLayoutResponsiveOrientationMaximizedMatrixAndLiveRetranslation() {
        InterfaceGrafica w;
        w.show();
        QCoreApplication::processEvents();

        // 1. Ativa Layout Duplo via QAction real
        QVERIFY(w.segundo_layout_action != nullptr);
        QVERIFY(!w.segundo_layout_action->isChecked());
        w.segundo_layout_action->trigger();
        QCoreApplication::processEvents();

        QVERIFY(w.segundo_layout_action->isChecked());
        QVERIFY(w.segundo_layout_widget != nullptr);
        QCOMPARE(w.checkboxes_2.size(), 6);
        QVERIFY(w.gerenciador_interface_2 != nullptr);
        QVERIFY(w.gerenciador_interface_2->folder_listbox != nullptr);

        // Marca modos distintos no Layout 1 e Layout 2
        w.checkboxes["rar"]->setChecked(true);
        w.checkboxes_2["7z"]->setChecked(true);
        w.checkboxes_2["extracao"]->setChecked(true);
        QCoreApplication::processEvents();
        QCOMPARE(w.current_layouts.size(), 1);
        QCOMPARE(w.current_layouts_2.size(), 2);
        QCOMPARE(w.scroll_area->verticalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);
        QCOMPARE(w.scroll_area_2->verticalScrollBarPolicy(), Qt::ScrollBarAsNeeded);

        // 2. Orientacao responsiva: paisagem -> QHBoxLayout, retrato (win_h > 1.5 * win_w) -> QVBoxLayout
        w.resize(1250, 720);
        QCoreApplication::processEvents();
        QVERIFY(qobject_cast<QHBoxLayout*>(w.columns_widget->layout()) != nullptr);

        w.resize(700, 1200); // min width = 700; 1200 > 1.5 * 700 (1050)
        QCoreApplication::processEvents();
        QVERIFY2(qobject_cast<QVBoxLayout*>(w.columns_widget->layout()) != nullptr,
                 "Portrait resize (700x1200) did not switch columns_widget layout to QVBoxLayout");

        w.resize(1200, 750);
        QCoreApplication::processEvents();
        QVERIFY(qobject_cast<QHBoxLayout*>(w.columns_widget->layout()) != nullptr);

        // 3. Fluxos com janela maximizada + alternancia repetida do Segundo Layout
        w.showMaximized();
        QCoreApplication::processEvents();
        QVERIFY(w.isMaximized());

        for (int cycle = 0; cycle < 3; ++cycle) {
            w.segundo_layout_action->trigger(); // desativa
            QCoreApplication::processEvents();
            QVERIFY(w.segundo_layout_widget == nullptr);
            QVERIFY(w.checkboxes_2.isEmpty());
            QVERIFY(w.current_layouts_2.isEmpty());
            QVERIFY(w.gerenciador_interface_2->folder_listbox == nullptr);

            w.segundo_layout_action->trigger(); // reativa mantendo maximizacao
            QCoreApplication::processEvents();
            QVERIFY(w.segundo_layout_widget != nullptr);
            QCOMPARE(w.checkboxes_2.size(), 6);
            QVERIFY(w.gerenciador_interface_2->folder_listbox != nullptr);
        }

        // 4. Troca de idioma ao vivo com ManualDialog, SobreDialog e Layout Duplo abertos
        w.manual_action->trigger();
        w.sobre_action->trigger();
        QCoreApplication::processEvents();
        QVERIFY(!w.dialog_manual.isNull());
        QVERIFY(!w.dialog_sobre.isNull());
        QVERIFY(w.dialog_manual->windowTitle().contains("Edição Full"));

        w.mudar_idioma("en_US");
        QCoreApplication::processEvents();
        QCOMPARE(w.arquivo_menu->title(), QStringLiteral("File"));
        QCOMPARE(w.checkboxes["extracao"]->text(), QStringLiteral("EXTRACTION"));
        QCOMPARE(w.checkboxes_2["extracao"]->text(), QStringLiteral("EXTRACTION"));
        QVERIFY(w.dialog_manual->windowTitle().contains("Full Edition"));
        QVERIFY(w.dialog_sobre->windowTitle().startsWith("About"));

        w.mudar_idioma("pt_BR");
        QCoreApplication::processEvents();
        QCOMPARE(w.arquivo_menu->title(), QStringLiteral("Arquivo"));
        QCOMPARE(w.checkboxes["extracao"]->text(), QStringLiteral("EXTRAÇÃO"));
        QVERIFY(w.dialog_manual->windowTitle().contains("Edição Full"));

        // 5. Encerramento limpo fecha dialogos filhos sem erro
        w.close();
        QCoreApplication::processEvents();
        QVERIFY(w.dialog_manual.isNull() || !w.dialog_manual->isVisible());
        QVERIFY(w.dialog_sobre.isNull() || !w.dialog_sobre->isVisible());
    }
};

CM_TEST_MAIN(TestUiFullEdition)
#ifndef __INTELLISENSE__
#include "test_08_ui_full_edition.moc"
#endif
