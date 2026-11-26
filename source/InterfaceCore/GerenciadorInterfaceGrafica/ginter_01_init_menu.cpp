#include "ginter_01_init_menu.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_20_exibir_sobre.hpp"
#include "source/GerenciamentoUI/ui_07_Manual.hpp"
#include "source/utils/LogManager.hpp"

#include <QCoreApplication>
#include <QFileDialog>

static bool is_layout2_active(InterfaceGrafica* self) {
    return self->segundo_layout_action && self->segundo_layout_action->isChecked() && self->gerenciador_interface_2;
}

static void menu_browse_folder(InterfaceGrafica* self) {
    if (is_layout2_active(self)) {
        self->gerenciador_interface->_browse(QFileDialog::Directory, { self->gerenciador_interface->folder_listbox, self->gerenciador_interface_2->folder_listbox });
    } else {
        self->gerenciador_interface->browse_folder();
    }
}

static void menu_browse_file(InterfaceGrafica* self) {
    if (is_layout2_active(self)) {
        self->gerenciador_interface->_browse(QFileDialog::ExistingFiles, { self->gerenciador_interface->folder_listbox, self->gerenciador_interface_2->folder_listbox });
    } else {
        self->gerenciador_interface->browse_file();
    }
}

static void menu_testar_integridade(InterfaceGrafica* self) {
    self->gerenciador_interface->testar_integridade();
    if (is_layout2_active(self)) {
        self->gerenciador_interface_2->testar_integridade();
    }
}

static void menu_clear_folders(InterfaceGrafica* self) {
    self->gerenciador_interface->clear_folders();
    if (is_layout2_active(self)) {
        self->gerenciador_interface_2->clear_folders();
    }
}

static void menu_clear_output(InterfaceGrafica* self) {
    self->gerenciador_interface->clear_output();
    if (is_layout2_active(self)) {
        self->gerenciador_interface_2->clear_output();
    }
}

static void menu_specify_output(InterfaceGrafica* self, const QString& method_name) {
    if (is_layout2_active(self)) {
        DragDropListWidget* lb1 = nullptr;
        DragDropListWidget* lb2 = nullptr;
        if (method_name == "rar") { lb1 = self->gerenciador_interface->output_listbox_rar; lb2 = self->gerenciador_interface_2->output_listbox_rar; }
        else if (method_name == "zip") { lb1 = self->gerenciador_interface->output_listbox_zip; lb2 = self->gerenciador_interface_2->output_listbox_zip; }
        else if (method_name == "7z") { lb1 = self->gerenciador_interface->output_listbox_7z; lb2 = self->gerenciador_interface_2->output_listbox_7z; }
        else if (method_name == "tar") { lb1 = self->gerenciador_interface->output_listbox_tar; lb2 = self->gerenciador_interface_2->output_listbox_tar; }
        else if (method_name == "wim") { lb1 = self->gerenciador_interface->output_listbox_wim; lb2 = self->gerenciador_interface_2->output_listbox_wim; }
        else if (method_name == "extracao") { lb1 = self->gerenciador_interface->output_listbox_extracao; lb2 = self->gerenciador_interface_2->output_listbox_extracao; }

        if (lb1 && lb2) {
            self->gerenciador_interface->_browse(QFileDialog::Directory, { lb1, lb2 });
        }
    } else {
        if (method_name == "rar") self->gerenciador_interface->output_button_output_RAR_clicked();
        else if (method_name == "zip") self->gerenciador_interface->output_button_output_ZIP_clicked();
        else if (method_name == "7z") self->gerenciador_interface->output_button_output_7Z_clicked();
        else if (method_name == "tar") self->gerenciador_interface->output_button_output_TAR_clicked();
        else if (method_name == "wim") self->gerenciador_interface->output_button_output_WIM_clicked();
        else if (method_name == "extracao") self->gerenciador_interface->output_button_output_EXTRACAO_clicked();
    }
}

static void menu_store_as(InterfaceGrafica* self, const QString& method_name) {
    if (method_name == "rar") self->gerenciador_interface->store_as_rar();
    else if (method_name == "zip") self->gerenciador_interface->store_as_zip();
    else if (method_name == "7z") self->gerenciador_interface->store_as_7z();
    else if (method_name == "tar") self->gerenciador_interface->store_as_tar();
    else if (method_name == "wim") self->gerenciador_interface->store_as_wim();

    if (is_layout2_active(self)) {
        if (method_name == "rar") self->gerenciador_interface_2->store_as_rar();
        else if (method_name == "zip") self->gerenciador_interface_2->store_as_zip();
        else if (method_name == "7z") self->gerenciador_interface_2->store_as_7z();
        else if (method_name == "tar") self->gerenciador_interface_2->store_as_tar();
        else if (method_name == "wim") self->gerenciador_interface_2->store_as_wim();
    }
}

static void menu_extract_files(InterfaceGrafica* self) {
    self->gerenciador_interface->extract_files();
    if (is_layout2_active(self)) {
        self->gerenciador_interface_2->extract_files();
    }
}

static void menu_clear_output_listbox(InterfaceGrafica* self, const QString& method_name) {
    if (method_name == "rar") self->gerenciador_interface->clear_output_listbox_rar();
    else if (method_name == "zip") self->gerenciador_interface->clear_output_listbox_zip();
    else if (method_name == "7z") self->gerenciador_interface->clear_output_listbox_7z();
    else if (method_name == "tar") self->gerenciador_interface->clear_output_listbox_tar();
    else if (method_name == "wim") self->gerenciador_interface->clear_output_listbox_wim();
    else if (method_name == "extracao") self->gerenciador_interface->clear_output_listbox_extracao();

    if (is_layout2_active(self)) {
        if (method_name == "rar") self->gerenciador_interface_2->clear_output_listbox_rar();
        else if (method_name == "zip") self->gerenciador_interface_2->clear_output_listbox_zip();
        else if (method_name == "7z") self->gerenciador_interface_2->clear_output_listbox_7z();
        else if (method_name == "tar") self->gerenciador_interface_2->clear_output_listbox_tar();
        else if (method_name == "wim") self->gerenciador_interface_2->clear_output_listbox_wim();
        else if (method_name == "extracao") self->gerenciador_interface_2->clear_output_listbox_extracao();
    }
}

void init_menu(InterfaceGrafica* self) {
    self->menu_bar = self->menuBar();

    self->arquivo_menu = self->menu_bar->addMenu(QCoreApplication::translate("InterfaceGrafica", "Arquivo"));

    self->adicionar_pastas_action = new QAction(QCoreApplication::translate("InterfaceGrafica", "Adicionar Pastas") + ":", self);
    QObject::connect(self->adicionar_pastas_action, &QAction::triggered, [self]() { menu_browse_folder(self); });
    self->arquivo_menu->addAction(self->adicionar_pastas_action);

    self->adicionar_arquivos_action = new QAction(QCoreApplication::translate("InterfaceGrafica", "Adicionar Arquivos") + ":", self);
    QObject::connect(self->adicionar_arquivos_action, &QAction::triggered, [self]() { menu_browse_file(self); });
    self->arquivo_menu->addAction(self->adicionar_arquivos_action);

    self->testar_integridade_action = new QAction(QCoreApplication::translate("InterfaceGrafica", "Testar Integridade") + ":", self);
    QObject::connect(self->testar_integridade_action, &QAction::triggered, [self]() { menu_testar_integridade(self); });
    self->arquivo_menu->addAction(self->testar_integridade_action);

    self->arquivo_menu->addSeparator();

    self->limpar_entrada_action = new QAction(QCoreApplication::translate("InterfaceGrafica", "Limpar Entrada") + ":", self);
    QObject::connect(self->limpar_entrada_action, &QAction::triggered, [self]() { menu_clear_folders(self); });
    self->arquivo_menu->addAction(self->limpar_entrada_action);

    self->limpar_todas_saidas_action = new QAction(QCoreApplication::translate("InterfaceGrafica", "Limpar Todas Saídas") + ":", self);
    QObject::connect(self->limpar_todas_saidas_action, &QAction::triggered, [self]() { menu_clear_output(self); });
    self->arquivo_menu->addAction(self->limpar_todas_saidas_action);

    self->arquivo_menu->addSeparator();

    self->especificar_saida_menu = self->arquivo_menu->addMenu(QCoreApplication::translate("InterfaceGrafica", "Especificar Saída") + ":");
#if ENABLE_RAR_SUPPORT
    self->especificar_rar_action = new QAction(".RAR", self);
    QObject::connect(self->especificar_rar_action, &QAction::triggered, [self]() { menu_specify_output(self, "rar"); });
    self->especificar_saida_menu->addAction(self->especificar_rar_action);
#else
    self->especificar_rar_action = nullptr;
#endif

    self->especificar_zip_action = new QAction(".ZIP", self);
    QObject::connect(self->especificar_zip_action, &QAction::triggered, [self]() { menu_specify_output(self, "zip"); });
    self->especificar_saida_menu->addAction(self->especificar_zip_action);

    self->especificar_7z_action = new QAction(".7Z", self);
    QObject::connect(self->especificar_7z_action, &QAction::triggered, [self]() { menu_specify_output(self, "7z"); });
    self->especificar_saida_menu->addAction(self->especificar_7z_action);

    self->especificar_tar_action = new QAction(".TAR", self);
    QObject::connect(self->especificar_tar_action, &QAction::triggered, [self]() { menu_specify_output(self, "tar"); });
    self->especificar_saida_menu->addAction(self->especificar_tar_action);

    self->especificar_wim_action = new QAction(".WIM", self);
    QObject::connect(self->especificar_wim_action, &QAction::triggered, [self]() { menu_specify_output(self, "wim"); });
    self->especificar_saida_menu->addAction(self->especificar_wim_action);

    self->especificar_extracao_action = new QAction(QCoreApplication::translate("InterfaceGrafica", "Extração"), self);
    QObject::connect(self->especificar_extracao_action, &QAction::triggered, [self]() { menu_specify_output(self, "extracao"); });
    self->especificar_saida_menu->addAction(self->especificar_extracao_action);

    self->arquivo_menu->addSeparator();

    self->armazenar_menu = self->arquivo_menu->addMenu(QCoreApplication::translate("InterfaceGrafica", "Armazenar") + ":");
#if ENABLE_RAR_SUPPORT
    self->armazenar_rar_action = new QAction(".RAR", self);
    QObject::connect(self->armazenar_rar_action, &QAction::triggered, [self]() { menu_store_as(self, "rar"); });
    self->armazenar_menu->addAction(self->armazenar_rar_action);
#else
    self->armazenar_rar_action = nullptr;
#endif

    self->armazenar_zip_action = new QAction(".ZIP", self);
    QObject::connect(self->armazenar_zip_action, &QAction::triggered, [self]() { menu_store_as(self, "zip"); });
    self->armazenar_menu->addAction(self->armazenar_zip_action);

    self->armazenar_7z_action = new QAction(".7Z", self);
    QObject::connect(self->armazenar_7z_action, &QAction::triggered, [self]() { menu_store_as(self, "7z"); });
    self->armazenar_menu->addAction(self->armazenar_7z_action);

    self->armazenar_tar_action = new QAction(".TAR", self);
    QObject::connect(self->armazenar_tar_action, &QAction::triggered, [self]() { menu_store_as(self, "tar"); });
    self->armazenar_menu->addAction(self->armazenar_tar_action);

    self->armazenar_wim_action = new QAction(".WIM", self);
    QObject::connect(self->armazenar_wim_action, &QAction::triggered, [self]() { menu_store_as(self, "wim"); });
    self->armazenar_menu->addAction(self->armazenar_wim_action);

    self->arquivo_menu->addSeparator();

    self->extrair_arquivos_action = new QAction(QCoreApplication::translate("InterfaceGrafica", "Extrair Arquivos") + ":", self);
    QObject::connect(self->extrair_arquivos_action, &QAction::triggered, [self]() { menu_extract_files(self); });
    self->arquivo_menu->addAction(self->extrair_arquivos_action);

    self->arquivo_menu->addSeparator();

    self->limpar_saida_menu = self->arquivo_menu->addMenu(QCoreApplication::translate("InterfaceGrafica", "Limpar Saída") + ":");
#if ENABLE_RAR_SUPPORT
    self->limpar_saida_rar_action = new QAction(".RAR", self);
    QObject::connect(self->limpar_saida_rar_action, &QAction::triggered, [self]() { menu_clear_output_listbox(self, "rar"); });
    self->limpar_saida_menu->addAction(self->limpar_saida_rar_action);
#else
    self->limpar_saida_rar_action = nullptr;
#endif

    self->limpar_saida_zip_action = new QAction(".ZIP", self);
    QObject::connect(self->limpar_saida_zip_action, &QAction::triggered, [self]() { menu_clear_output_listbox(self, "zip"); });
    self->limpar_saida_menu->addAction(self->limpar_saida_zip_action);

    self->limpar_saida_7z_action = new QAction(".7Z", self);
    QObject::connect(self->limpar_saida_7z_action, &QAction::triggered, [self]() { menu_clear_output_listbox(self, "7z"); });
    self->limpar_saida_menu->addAction(self->limpar_saida_7z_action);

    self->limpar_saida_tar_action = new QAction(".TAR", self);
    QObject::connect(self->limpar_saida_tar_action, &QAction::triggered, [self]() { menu_clear_output_listbox(self, "tar"); });
    self->limpar_saida_menu->addAction(self->limpar_saida_tar_action);

    self->limpar_saida_wim_action = new QAction(".WIM", self);
    QObject::connect(self->limpar_saida_wim_action, &QAction::triggered, [self]() { menu_clear_output_listbox(self, "wim"); });
    self->limpar_saida_menu->addAction(self->limpar_saida_wim_action);

    self->limpar_saida_extracao_action = new QAction(QCoreApplication::translate("InterfaceGrafica", "Extração"), self);
    QObject::connect(self->limpar_saida_extracao_action, &QAction::triggered, [self]() { menu_clear_output_listbox(self, "extracao"); });
    self->limpar_saida_menu->addAction(self->limpar_saida_extracao_action);

    self->arquivo_menu->addSeparator();

    self->sair_action = new QAction(QCoreApplication::translate("InterfaceGrafica", "Sair"), self);
    QObject::connect(self->sair_action, &QAction::triggered, self, &QWidget::close);
    self->arquivo_menu->addAction(self->sair_action);

    self->config_menu = self->menu_bar->addMenu(QCoreApplication::translate("InterfaceGrafica", "Configurações"));

    self->compression_method_action = new QAction(QCoreApplication::translate("InterfaceGrafica", "Método de Compressão"), self);
    QObject::connect(self->compression_method_action, &QAction::triggered, [self]() { self->select_compression_method(); });
    self->config_menu->addAction(self->compression_method_action);
    self->select_compression_method();

    self->layouts_menu = new MenuPersistente(QCoreApplication::translate("InterfaceGrafica", "Layouts"), self);
    self->config_menu->addMenu(self->layouts_menu);

    self->primeiro_layout_action = new QAction(QCoreApplication::translate("InterfaceGrafica", "Primeiro Layout"), self);
    self->primeiro_layout_action->setCheckable(true);
    self->primeiro_layout_action->setChecked(true);
    self->primeiro_layout_action->setEnabled(false);
    self->layouts_menu->addAction(self->primeiro_layout_action);

    self->segundo_layout_action = new QAction(QCoreApplication::translate("InterfaceGrafica", "Segundo Layout"), self);
    self->segundo_layout_action->setCheckable(true);
    self->segundo_layout_action->setChecked(false);
    QObject::connect(self->segundo_layout_action, &QAction::triggered, [self]() { self->toggle_segundo_layout(); });
    self->layouts_menu->addAction(self->segundo_layout_action);

    self->idiomas_menu = new MenuPersistente(QCoreApplication::translate("InterfaceGrafica", "Idiomas"), self);
    self->config_menu->addMenu(self->idiomas_menu);

    self->pt_br_action = new QAction("Português (Brasil)", self);
    self->pt_br_action->setCheckable(true);
    self->pt_br_action->setChecked(self->gerenciador_traducao->idioma_atual == "pt_BR");
    QObject::connect(self->pt_br_action, &QAction::triggered, [self]() { self->mudar_idioma("pt_BR"); });
    self->idiomas_menu->addAction(self->pt_br_action);

    self->en_us_action = new QAction("English (United States)", self);
    self->en_us_action->setCheckable(true);
    self->en_us_action->setChecked(self->gerenciador_traducao->idioma_atual == "en_US");
    QObject::connect(self->en_us_action, &QAction::triggered, [self]() { self->mudar_idioma("en_US"); });
    self->idiomas_menu->addAction(self->en_us_action);

    self->opcoes_menu = self->menu_bar->addMenu(QCoreApplication::translate("InterfaceGrafica", "Opções"));

    self->manual_action = new QAction(QCoreApplication::translate("InterfaceGrafica", "Manual"), self);
    QObject::connect(self->manual_action, &QAction::triggered, [self]() { exibir_manual(self); });
    self->opcoes_menu->addAction(self->manual_action);

    self->sobre_action = new QAction(QCoreApplication::translate("InterfaceGrafica", "Sobre"), self);
    QObject::connect(self->sobre_action, &QAction::triggered, [self]() { exibir_sobre(self); });
    self->opcoes_menu->addAction(self->sobre_action);

    QObject::connect(self->config_menu, &QMenu::aboutToShow, [self]() { self->update_compression_menus(); });
}
