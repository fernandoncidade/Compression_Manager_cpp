#include "ginter_24_create_second_quadrant_2.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/GerenciamentoUI/ui_03_dragDrop.hpp"
#include "source/utils/LogManager.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QCoreApplication>

QVBoxLayout* ginter_create_second_quadrant_2(InterfaceGrafica* self) {
    auto layout = new QVBoxLayout();
    auto button_layout = new QHBoxLayout();

    self->folder_label_2 = new QLabel(QCoreApplication::translate("InterfaceGrafica", "Diretório(s) Pastas e Arquivos:"));
    button_layout->addWidget(self->folder_label_2);

    button_layout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));

    self->clear_input_label_2 = new QLabel(QCoreApplication::translate("InterfaceGrafica", "Limpar Entrada") + ":");
    auto clear_input_button_2 = self->create_button("", [self]() {
        self->gerenciador_interface_2->clear_folders();
    }, "clear_button3.png");
    self->main_buttons_2[self->clear_input_label_2] = "Limpar Entrada:";

    self->clear_outputs_label_2 = new QLabel(QCoreApplication::translate("InterfaceGrafica", "Limpar Todas Saídas") + ":");
    auto clear_outputs_button_2 = self->create_button("", [self]() {
        self->gerenciador_interface_2->clear_output();
    }, "clear_button2.png");
    self->main_buttons_2[self->clear_outputs_label_2] = "Limpar Todas Saídas:";

    button_layout->addWidget(self->clear_input_label_2);
    button_layout->addWidget(clear_input_button_2);
    button_layout->addWidget(self->clear_outputs_label_2);
    button_layout->addWidget(clear_outputs_button_2);

    layout->addLayout(button_layout);
    if (!self->gerenciador_interface_2->folder_listbox) {
        self->gerenciador_interface_2->folder_listbox = new DragDropListWidget(self, false);
    }
    layout->addWidget(self->gerenciador_interface_2->folder_listbox);

    auto retranslate = [self]() {
        try {
            if (self->folder_label_2) self->folder_label_2->setText(QCoreApplication::translate("InterfaceGrafica", "Diretório(s) Pastas e Arquivos:"));
            if (self->clear_input_label_2) self->clear_input_label_2->setText(QCoreApplication::translate("InterfaceGrafica", "Limpar Entrada") + ":");
            if (self->clear_outputs_label_2) self->clear_outputs_label_2->setText(QCoreApplication::translate("InterfaceGrafica", "Limpar Todas Saídas") + ":");

            if (self->clear_input_label_2) self->main_buttons_2[self->clear_input_label_2] = QCoreApplication::translate("InterfaceGrafica", "Limpar Entrada") + ":";
            if (self->clear_outputs_label_2) self->main_buttons_2[self->clear_outputs_label_2] = QCoreApplication::translate("InterfaceGrafica", "Limpar Todas Saídas") + ":";
        } catch (const std::exception& e) {
            LogManager::instance().log(QString("Erro em retranslate (second quadrant 2): %1").arg(e.what()), "ERROR");
        }
    };

    if (self->gerenciador_traducao && self->folder_label_2) {
        QObject::connect(self->gerenciador_traducao.get(), &GerenciadorTraducao::idioma_alterado, self->folder_label_2, retranslate);
        retranslate();
    }

    return layout;
}
