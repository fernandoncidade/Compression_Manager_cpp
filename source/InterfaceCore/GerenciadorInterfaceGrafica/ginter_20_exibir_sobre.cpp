#include "ginter_20_exibir_sobre.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/GerenciamentoUI/ui_05_OpcoesSobre.hpp"
#include "source/GerenciamentoUI/ui_06_SobreDialog.hpp"
#include "source/utils/LogManager.hpp"
#include <QCoreApplication>
#include <QMessageBox>

static QString get_text(const char* text) {
    return QCoreApplication::translate("InterfaceGrafica", text);
}

struct SobreDados {
    QString titulo;
    QString cabecalho_fixo;
    QString texto_history;
    QString texto_sobre;
    QString texto_licenca;
    QString site_licencas;
    QString show_history_text;
    QString hide_history_text;
    QString show_details_text;
    QString hide_details_text;
    QString show_licenses_text;
    QString hide_licenses_text;
    QString ok_text;
    QString site_oficial_text;
    QString texto_aviso;
    QString show_notices_text;
    QString hide_notices_text;
    QString texto_privacidade;
    QString show_privacy_policy_text;
    QString hide_privacy_policy_text;
    QString info_not_available_text;
    QString texto_release_notes;
    QString show_release_notes_text;
    QString hide_release_notes_text;
};

static SobreDados preparar_dados_sobre(InterfaceGrafica* self) {
    OpcoesSobre::carregar_textos_sobre();
    QString idioma = self->gerenciador_traducao ? self->gerenciador_traducao->obter_idioma_atual() : "pt_BR";

    QString texto_sobre = (idioma == "pt_BR") ? OpcoesSobre::ABOUT_TEXT_PT_BR : OpcoesSobre::ABOUT_TEXT_EN_US;
    QString texto_licenca = (idioma == "pt_BR") ? OpcoesSobre::LICENSE_TEXT_PT_BR : OpcoesSobre::LICENSE_TEXT_EN_US;
    QString texto_aviso = (idioma == "pt_BR") ? OpcoesSobre::NOTICE_TEXT_PT_BR : OpcoesSobre::NOTICE_TEXT_EN_US;
    QString texto_privacidade = (idioma == "pt_BR") ? OpcoesSobre::Privacy_Policy_pt_BR : OpcoesSobre::Privacy_Policy_en_US;
    QString texto_history = (idioma == "pt_BR") ? OpcoesSobre::History_APP_pt_BR : OpcoesSobre::History_APP_en_US;
    QString texto_release_notes = (idioma == "pt_BR") ? OpcoesSobre::RELEASE_NOTES_pt_BR : OpcoesSobre::RELEASE_NOTES_en_US;

#if ENABLE_RAR_SUPPORT
    QString texto_descricao = (idioma == "pt_BR")
        ? "COMPRESSION MANAGER é um utilitário gráfico leve para Windows projetado para "
          "empacotamento, extração e verificação de integridade de arquivos e pastas. "
          "Oferece suporte a múltiplos formatos (RAR, ZIP, 7Z, TAR, WIM e outros reconhecidos "
          "pelo 7‑Zip), seleção de vários diretórios de saída, métodos de compressão "
          "configuráveis, fila de processos e operação via arrastar-e-soltar. A interface "
          "é multilíngue (pt_BR / en_US) e preparada para empacotamento como executável "
          "(distribuição sem necessidade de instalação manual de dependências)."
        : "COMPRESSION MANAGER is a lightweight graphical utility for Windows designed "
          "for packing, extracting and integrity checking of files and folders. It "
          "supports multiple formats (RAR, ZIP, 7Z, TAR, WIM and others recognized by 7‑Zip), "
          "selection of multiple output directories, configurable compression methods, "
          "process queuing and drag-and-drop operation. The interface is multilingual "
          "(pt_BR / en_US) and prepared for packaging as an executable (distribution "
          "without requiring manual installation of dependencies).";
#else
    QString texto_descricao = (idioma == "pt_BR")
        ? "COMPRESSION MANAGER é um utilitário gráfico leve para Windows projetado para "
          "empacotamento, extração e verificação de integridade de arquivos e pastas. "
          "Oferece suporte a múltiplos formatos (ZIP, 7Z, TAR, WIM e outros reconhecidos "
          "pelo 7‑Zip), seleção de vários diretórios de saída, métodos de compressão "
          "configuráveis, fila de processos e operação via arrastar-e-soltar. A interface "
          "é multilíngue (pt_BR / en_US) e preparada para empacotamento como executável "
          "(distribuição sem necessidade de instalação manual de dependências)."
        : "COMPRESSION MANAGER is a lightweight graphical utility for Windows designed "
          "for packing, extracting and integrity checking of files and folders. It "
          "supports multiple formats (ZIP, 7Z, TAR, WIM and others recognized by 7‑Zip), "
          "selection of multiple output directories, configurable compression methods, "
          "process queuing and drag-and-drop operation. The interface is multilingual "
          "(pt_BR / en_US) and prepared for packaging as an executable (distribution "
          "without requiring manual installation of dependencies).";
#endif

    if (!OpcoesSobre::LICENSE_OWNER.isEmpty()) {
        texto_licenca += "\n\n" + OpcoesSobre::LICENSE_OWNER;
    }

    QString cabecalho_fixo = QString(
        "<h3>COMPRESSION MANAGER</h3>"
        "<p><b>%1:</b> 2026.11.22.0</p>"
        "<p><b>%2:</b> Fernando Nillsson Cidade</p>"
        "<p><b>%3:</b> %4</p>")
        .arg(get_text("Versão"),
             get_text("Autores"),
             get_text("Descrição"),
             texto_descricao);

    return {
        QString("%1 - COMPRESSION MANAGER").arg(get_text("Sobre")),
        cabecalho_fixo,
        texto_history,
        texto_sobre,
        texto_licenca,
        OpcoesSobre::SITE_LICENSES,
        get_text("Histórico"),
        get_text("Ocultar histórico"),
        get_text("Detalhes"),
        get_text("Ocultar detalhes"),
        get_text("Licenças"),
        get_text("Ocultar licenças"),
        QCoreApplication::translate("Dialog", "OK"),
        get_text("Site oficial"),
        texto_aviso,
        get_text("Avisos"),
        get_text("Ocultar avisos"),
        texto_privacidade,
        get_text("Política de Privacidade"),
        get_text("Ocultar política de privacidade"),
        get_text("Informação não disponível"),
        texto_release_notes,
        get_text("Notas de Versão"),
        get_text("Ocultar notas de versão")
    };
}

void ginter_exibir_sobre(InterfaceGrafica* self) {
    try {
        if (self->dialog_sobre) {
            self->dialog_sobre->showNormal();
            self->dialog_sobre->raise();
            self->dialog_sobre->activateWindow();
            return;
        }

        SobreDados dados = preparar_dados_sobre(self);

        auto dialog = new SobreDialog(
            nullptr,
            dados.titulo,
            dados.cabecalho_fixo,
            dados.texto_history,
            dados.texto_sobre,
            dados.texto_licenca,
            dados.site_licencas,
            dados.show_history_text,
            dados.hide_history_text,
            dados.show_details_text,
            dados.hide_details_text,
            dados.show_licenses_text,
            dados.hide_licenses_text,
            dados.ok_text,
            dados.site_oficial_text,
            dados.texto_aviso,
            dados.show_notices_text,
            dados.hide_notices_text,
            dados.texto_privacidade,
            dados.show_privacy_policy_text,
            dados.hide_privacy_policy_text,
            dados.info_not_available_text,
            dados.texto_release_notes,
            dados.show_release_notes_text,
            dados.hide_release_notes_text
        );

        self->dialog_sobre = dialog;
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->resize(900, 500);
        dialog->showNormal();
        dialog->raise();
        dialog->activateWindow();
    } catch (const std::exception& e) {
        QMessageBox erro_msg(QMessageBox::Critical, get_text("Erro"), QString("%1: %2").arg(get_text("Erro"), e.what()), QMessageBox::Ok, self);
        erro_msg.setWindowModality(Qt::ApplicationModal);
        erro_msg.setWindowFlag(Qt::WindowStaysOnTopHint, true);
        erro_msg.raise();
        erro_msg.activateWindow();
        erro_msg.exec();
    }
}

void ginter_atualizar_sobre(InterfaceGrafica* self) {
    if (self && self->dialog_sobre) {
        try {
            SobreDados dados = preparar_dados_sobre(self);
            self->dialog_sobre->atualizar_conteudo(
                dados.titulo,
                dados.cabecalho_fixo,
                dados.texto_history,
                dados.texto_sobre,
                dados.texto_licenca,
                dados.site_licencas,
                dados.show_history_text,
                dados.hide_history_text,
                dados.show_details_text,
                dados.hide_details_text,
                dados.show_licenses_text,
                dados.hide_licenses_text,
                dados.ok_text,
                dados.site_oficial_text,
                dados.texto_aviso,
                dados.show_notices_text,
                dados.hide_notices_text,
                dados.texto_privacidade,
                dados.show_privacy_policy_text,
                dados.hide_privacy_policy_text,
                dados.info_not_available_text,
                dados.texto_release_notes,
                dados.show_release_notes_text,
                dados.hide_release_notes_text
            );
        } catch (const std::exception& e) {
            LogManager::instance().log(QString("Erro ao atualizar diálogo Sobre: %1").arg(e.what()), "ERROR");
        }
    }
}
