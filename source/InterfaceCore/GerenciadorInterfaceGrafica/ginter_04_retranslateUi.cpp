#include "ginter_04_retranslateUi.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/InterfaceCore/GerenciadorInterfaceGrafica/ginter_20_exibir_sobre.hpp"
#include "source/GerenciamentoUI/ui_07_Manual.hpp"
#include "source/utils/LogManager.hpp"
#include <QCoreApplication>

void retranslateUi(InterfaceGrafica* self) {
    self->setWindowTitle(QCoreApplication::translate("InterfaceGrafica", "Compression Manager"));

    if (self->arquivo_menu) {
        self->arquivo_menu->setTitle(QCoreApplication::translate("InterfaceGrafica", "Arquivo"));
    }
    if (self->adicionar_pastas_action) {
        self->adicionar_pastas_action->setText(QCoreApplication::translate("InterfaceGrafica", "Adicionar Pastas") + ":");
    }
    if (self->adicionar_arquivos_action) {
        self->adicionar_arquivos_action->setText(QCoreApplication::translate("InterfaceGrafica", "Adicionar Arquivos") + ":");
    }
    if (self->testar_integridade_action) {
        self->testar_integridade_action->setText(QCoreApplication::translate("InterfaceGrafica", "Testar Integridade") + ":");
    }
    if (self->limpar_entrada_action) {
        self->limpar_entrada_action->setText(QCoreApplication::translate("InterfaceGrafica", "Limpar Entrada") + ":");
    }
    if (self->limpar_todas_saidas_action) {
        self->limpar_todas_saidas_action->setText(QCoreApplication::translate("InterfaceGrafica", "Limpar Todas Saídas") + ":");
    }
    if (self->especificar_saida_menu) {
        self->especificar_saida_menu->setTitle(QCoreApplication::translate("InterfaceGrafica", "Especificar Saída") + ":");
    }
    if (self->especificar_extracao_action) {
        self->especificar_extracao_action->setText(QCoreApplication::translate("InterfaceGrafica", "Extração"));
    }
    if (self->especificar_rar_action) {
        self->especificar_rar_action->setText(QCoreApplication::translate("InterfaceGrafica", ".RAR"));
    }
    if (self->especificar_zip_action) {
        self->especificar_zip_action->setText(QCoreApplication::translate("InterfaceGrafica", ".ZIP"));
    }
    if (self->especificar_7z_action) {
        self->especificar_7z_action->setText(QCoreApplication::translate("InterfaceGrafica", ".7Z"));
    }
    if (self->especificar_tar_action) {
        self->especificar_tar_action->setText(QCoreApplication::translate("InterfaceGrafica", ".TAR"));
    }
    if (self->especificar_wim_action) {
        self->especificar_wim_action->setText(QCoreApplication::translate("InterfaceGrafica", ".WIM"));
    }
    if (self->armazenar_menu) {
        self->armazenar_menu->setTitle(QCoreApplication::translate("InterfaceGrafica", "Armazenar") + ":");
    }
    if (self->armazenar_rar_action) {
        self->armazenar_rar_action->setText(QCoreApplication::translate("InterfaceGrafica", ".RAR"));
    }
    if (self->armazenar_zip_action) {
        self->armazenar_zip_action->setText(QCoreApplication::translate("InterfaceGrafica", ".ZIP"));
    }
    if (self->armazenar_7z_action) {
        self->armazenar_7z_action->setText(QCoreApplication::translate("InterfaceGrafica", ".7Z"));
    }
    if (self->armazenar_tar_action) {
        self->armazenar_tar_action->setText(QCoreApplication::translate("InterfaceGrafica", ".TAR"));
    }
    if (self->armazenar_wim_action) {
        self->armazenar_wim_action->setText(QCoreApplication::translate("InterfaceGrafica", ".WIM"));
    }
    if (self->extrair_arquivos_action) {
        self->extrair_arquivos_action->setText(QCoreApplication::translate("InterfaceGrafica", "Extrair Arquivos") + ":");
    }
    if (self->limpar_saida_menu) {
        self->limpar_saida_menu->setTitle(QCoreApplication::translate("InterfaceGrafica", "Limpar Saída") + ":");
    }
    if (self->limpar_saida_rar_action) {
        self->limpar_saida_rar_action->setText(QCoreApplication::translate("InterfaceGrafica", ".RAR"));
    }
    if (self->limpar_saida_zip_action) {
        self->limpar_saida_zip_action->setText(QCoreApplication::translate("InterfaceGrafica", ".ZIP"));
    }
    if (self->limpar_saida_7z_action) {
        self->limpar_saida_7z_action->setText(QCoreApplication::translate("InterfaceGrafica", ".7Z"));
    }
    if (self->limpar_saida_tar_action) {
        self->limpar_saida_tar_action->setText(QCoreApplication::translate("InterfaceGrafica", ".TAR"));
    }
    if (self->limpar_saida_wim_action) {
        self->limpar_saida_wim_action->setText(QCoreApplication::translate("InterfaceGrafica", ".WIM"));
    }
    if (self->limpar_saida_extracao_action) {
        self->limpar_saida_extracao_action->setText(QCoreApplication::translate("InterfaceGrafica", "Extração"));
    }
    if (self->sair_action) {
        self->sair_action->setText(QCoreApplication::translate("InterfaceGrafica", "Sair"));
    }

    if (self->config_menu) {
        self->config_menu->setTitle(QCoreApplication::translate("InterfaceGrafica", "Configurações"));
    }
    if (self->idiomas_menu) {
        self->idiomas_menu->setTitle(QCoreApplication::translate("InterfaceGrafica", "Idiomas"));
    }
    if (self->compression_method_action) {
        self->compression_method_action->setText(QCoreApplication::translate("InterfaceGrafica", "Método de Compressão"));
    }
    if (self->opcoes_menu) {
        self->opcoes_menu->setTitle(QCoreApplication::translate("InterfaceGrafica", "Opções"));
    }
    if (self->manual_action) {
        self->manual_action->setText(QCoreApplication::translate("InterfaceGrafica", "Manual"));
    }
    if (self->sobre_action) {
        self->sobre_action->setText(QCoreApplication::translate("InterfaceGrafica", "Sobre"));
    }
    if (self->layouts_menu) {
        self->layouts_menu->setTitle(QCoreApplication::translate("InterfaceGrafica", "Layouts"));
    }
    if (self->primeiro_layout_action) {
        self->primeiro_layout_action->setText(QCoreApplication::translate("InterfaceGrafica", "Primeiro Layout"));
    }
    if (self->segundo_layout_action) {
        self->segundo_layout_action->setText(QCoreApplication::translate("InterfaceGrafica", "Segundo Layout"));
    }

    if (self->methods_group_2) {
        self->methods_group_2->setTitle(QCoreApplication::translate("InterfaceGrafica", "Métodos de compressão e extração"));
    }

    for (auto it = self->main_buttons.begin(); it != self->main_buttons.end(); ++it) {
        if (it.key()) {
            it.key()->setText(QCoreApplication::translate("InterfaceGrafica", it.value().toUtf8().constData()));
        }
    }

    if (self->folder_label) {
        self->folder_label->setText(QCoreApplication::translate("InterfaceGrafica", "Diretório(s) Pastas e Arquivos:"));
    }

    for (auto it = self->checkboxes.begin(); it != self->checkboxes.end(); ++it) {
        const QString& method = it.key();
        if (it.value()) {
            if (method == "extracao") {
                it.value()->setText(QCoreApplication::translate("InterfaceGrafica", "EXTRAÇÃO"));
            } else {
                it.value()->setText(method.toUpper());
            }
        }
    }

    if (self->methods_group) {
        self->methods_group->setTitle(QCoreApplication::translate("InterfaceGrafica", "Métodos de compressão e extração"));
    }

    for (auto it = self->main_buttons_2.begin(); it != self->main_buttons_2.end(); ++it) {
        if (it.key()) {
            it.key()->setText(QCoreApplication::translate("InterfaceGrafica", it.value().toUtf8().constData()));
        }
    }

    if (self->folder_label_2) {
        self->folder_label_2->setText(QCoreApplication::translate("InterfaceGrafica", "Diretório(s) Pastas e Arquivos:"));
    }

    for (auto it = self->checkboxes_2.begin(); it != self->checkboxes_2.end(); ++it) {
        const QString& method = it.key();
        if (it.value()) {
            if (method == "extracao") {
                it.value()->setText(QCoreApplication::translate("InterfaceGrafica", "EXTRAÇÃO"));
            } else {
                it.value()->setText(method.toUpper());
            }
        }
    }

    self->rebuild_method_layouts();
    if (self->gerenciador_interface) {
        self->gerenciador_interface->atualizar_traducoes_dialogos();
    }
    if (self->gerenciador_interface_2 && self->segundo_layout_action && self->segundo_layout_action->isChecked()) {
        self->gerenciador_interface_2->atualizar_traducoes_dialogos();
    }

    self->update_compression_menus();

    QString idioma_atual = self->gerenciador_traducao ? self->gerenciador_traducao->obter_idioma_atual() : "pt_BR";
    self->setProperty("idioma_atual", idioma_atual);

    if (self->dialog_manual) {
        self->dialog_manual->atualizar_traducao(idioma_atual);
    }
    if (self->dialog_sobre) {
        ginter_atualizar_sobre(self);
    }
}
