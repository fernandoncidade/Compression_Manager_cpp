#pragma once

#include <QMainWindow>
#include <QMap>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QLabel>
#include <QCheckBox>
#include <QScrollArea>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPointer>
#include <memory>

#include "source/GerenciamentoUI/ui_01_layoutsCompressao.hpp"
#include "source/GerenciamentoUI/ui_02_gerenteGUILayouts.hpp"
#include "source/MotoresCompressao/mtcomp_01_metodosCompressao.hpp"
#include "source/language/tr_01_gerenciadorTraducao.hpp"

class ManualDialog;
class SobreDialog;

class InterfaceGrafica : public QMainWindow, public MetodoCompressao {
    Q_OBJECT

public:
    explicit InterfaceGrafica(QWidget* parent = nullptr);
    ~InterfaceGrafica() override;

    void init_menu();
    void update_compression_menus();
    void mudar_idioma(const QString& codigo_idioma);
    void retranslateUi();
    void init_ui();

    QWidget* create_widget_with_layout(QLayout* layout);
    QHBoxLayout* create_main_layout_1();
    QVBoxLayout* create_first_quadrant();
    QVBoxLayout* create_second_quadrant();
    void rebuild_method_layouts();

    QHBoxLayout* create_main_layout_2();
    QVBoxLayout* create_third_quadrant();
    QGroupBox* create_scroll_area();
    QPushButton* create_button(const QString& text, std::function<void()> callback = nullptr, const QString& icon = QString());
    void create_method_checkboxes(QVBoxLayout* layout);
    void on_method_toggled();
    void adjust_scroll_area();
    void remove_layout_widgets(QLayout* layout);

    void toggle_segundo_layout();
    QGroupBox* create_scroll_area_2();
    QVBoxLayout* create_first_quadrant_2();
    QVBoxLayout* create_second_quadrant_2();
    void create_method_checkboxes_2(QVBoxLayout* layout);
    void on_method_toggled_2();
    void adapt_layout_orientation();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    void changeEvent(QEvent* event) override;

public:
    QPointer<ManualDialog> dialog_manual;
    QPointer<SobreDialog> dialog_sobre;

    std::unique_ptr<GerenciadorInterface> gerenciador_interface;
    std::unique_ptr<LayoutsCompressao> layouts_compressao;
    QMap<QString, LayoutsCompressao::LayoutCreatorFn> compression_method_layouts;
    QMap<QString, QLayout*> current_layouts;

    std::unique_ptr<GerenciadorInterface> gerenciador_interface_2;
    std::unique_ptr<LayoutsCompressao> layouts_compressao_2;
    QMap<QString, LayoutsCompressao::LayoutCreatorFn> compression_method_layouts_2;
    QMap<QString, QLayout*> current_layouts_2;
    QMap<QString, QCheckBox*> checkboxes_2;

    std::unique_ptr<GerenciadorTraducao> gerenciador_traducao;
    QMap<QWidget*, QString> traduzir_widgets;

    // Menus e Actions
    QMenuBar* menu_bar = nullptr;
    QMenu* arquivo_menu = nullptr;
    QAction* adicionar_pastas_action = nullptr;
    QAction* adicionar_arquivos_action = nullptr;
    QAction* testar_integridade_action = nullptr;
    QAction* limpar_entrada_action = nullptr;
    QAction* limpar_todas_saidas_action = nullptr;

    QMenu* especificar_saida_menu = nullptr;
    QAction* especificar_rar_action = nullptr;
    QAction* especificar_zip_action = nullptr;
    QAction* especificar_7z_action = nullptr;
    QAction* especificar_tar_action = nullptr;
    QAction* especificar_wim_action = nullptr;
    QAction* especificar_extracao_action = nullptr;

    QMenu* armazenar_menu = nullptr;
    QAction* armazenar_rar_action = nullptr;
    QAction* armazenar_zip_action = nullptr;
    QAction* armazenar_7z_action = nullptr;
    QAction* armazenar_tar_action = nullptr;
    QAction* armazenar_wim_action = nullptr;
    QAction* extrair_arquivos_action = nullptr;

    QMenu* limpar_saida_menu = nullptr;
    QAction* limpar_saida_rar_action = nullptr;
    QAction* limpar_saida_zip_action = nullptr;
    QAction* limpar_saida_7z_action = nullptr;
    QAction* limpar_saida_tar_action = nullptr;
    QAction* limpar_saida_wim_action = nullptr;
    QAction* limpar_saida_extracao_action = nullptr;

    QAction* sair_action = nullptr;

    MenuPersistente* layouts_menu = nullptr;
    QAction* primeiro_layout_action = nullptr;
    QAction* segundo_layout_action = nullptr;

    MenuPersistente* idiomas_menu = nullptr;
    QAction* pt_br_action = nullptr;
    QAction* en_us_action = nullptr;

    QMenu* opcoes_menu = nullptr;
    QAction* manual_action = nullptr;
    QAction* sobre_action = nullptr;

    // Widgets de layout
    QMap<QLabel*, QString> main_buttons;
    QMap<QLabel*, QString> main_buttons_2;
    QLabel* folder_label = nullptr;
    QLabel* folder_label_2 = nullptr;
    QLabel* add_folders_label = nullptr;
    QLabel* add_files_label = nullptr;
    QLabel* test_label = nullptr;
    QLabel* clear_input_label = nullptr;
    QLabel* clear_outputs_label = nullptr;

    QLabel* add_folders_label_2 = nullptr;
    QLabel* add_files_label_2 = nullptr;
    QLabel* test_label_2 = nullptr;
    QLabel* clear_input_label_2 = nullptr;
    QLabel* clear_outputs_label_2 = nullptr;

    QMap<QString, QCheckBox*> checkboxes;

    QVBoxLayout* primeiro_layout_column = nullptr;
    QWidget* main_layout_1_widget = nullptr;
    QWidget* primeiro_layout_widget = nullptr;

    QWidget* segundo_layout_widget = nullptr;
    QVBoxLayout* segundo_layout_column = nullptr;
    QWidget* main_layout_1_widget_2 = nullptr;

    QWidget* columns_widget = nullptr;

    QGroupBox* methods_group = nullptr;
    QScrollArea* scroll_area = nullptr;
    QWidget* scroll_area_widget = nullptr;
    QVBoxLayout* scroll_area_layout = nullptr;
    QVBoxLayout* method_layouts_container = nullptr;

    QGroupBox* methods_group_2 = nullptr;
    QScrollArea* scroll_area_2 = nullptr;
    QWidget* scroll_area_widget_2 = nullptr;
    QVBoxLayout* scroll_area_layout_2 = nullptr;
    QVBoxLayout* method_layouts_container_2 = nullptr;
};
