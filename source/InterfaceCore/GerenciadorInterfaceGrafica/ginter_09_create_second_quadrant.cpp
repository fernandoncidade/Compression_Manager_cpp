#include "ginter_09_create_second_quadrant.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/GerenciamentoUI/ui_03_dragDrop.hpp"
#include "source/utils/LogManager.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QCoreApplication>

QVBoxLayout* create_second_quadrant(InterfaceGrafica* self) {
    auto layout = new QVBoxLayout();
    auto button_layout = new QHBoxLayout();

    self->folder_label = new QLabel(QCoreApplication::translate("InterfaceGrafica", "Diretório(s) Pastas e Arquivos:"));
    button_layout->addWidget(self->folder_label);

    button_layout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));

    self->clear_input_label = new QLabel(QCoreApplication::translate("InterfaceGrafica", "Limpar Entrada") + ":");
    auto clear_input_button = self->create_button("", [self]() {
        self->gerenciador_interface->clear_folders();
    }, "clear_button3.png");
    self->main_buttons[self->clear_input_label] = "Limpar Entrada:";

    self->clear_outputs_label = new QLabel(QCoreApplication::translate("InterfaceGrafica", "Limpar Todas Saídas") + ":");
    auto clear_outputs_button = self->create_button("", [self]() {
        self->gerenciador_interface->clear_output();
    }, "clear_button2.png");
    self->main_buttons[self->clear_outputs_label] = "Limpar Todas Saídas:";

    button_layout->addWidget(self->clear_input_label);
    button_layout->addWidget(clear_input_button);
    button_layout->addWidget(self->clear_outputs_label);
    button_layout->addWidget(clear_outputs_button);

    layout->addLayout(button_layout);
    if (!self->gerenciador_interface->folder_listbox) {
        self->gerenciador_interface->folder_listbox = new DragDropListWidget(self, false);
    }
    layout->addWidget(self->gerenciador_interface->folder_listbox);

    auto retranslate = [self]() {
        try {
            if (self->folder_label) self->folder_label->setText(QCoreApplication::translate("InterfaceGrafica", "Diretório(s) Pastas e Arquivos:"));
            if (self->clear_input_label) self->clear_input_label->setText(QCoreApplication::translate("InterfaceGrafica", "Limpar Entrada") + ":");
            if (self->clear_outputs_label) self->clear_outputs_label->setText(QCoreApplication::translate("InterfaceGrafica", "Limpar Todas Saídas") + ":");

            if (self->clear_input_label) self->main_buttons[self->clear_input_label] = QCoreApplication::translate("InterfaceGrafica", "Limpar Entrada") + ":";
            if (self->clear_outputs_label) self->main_buttons[self->clear_outputs_label] = QCoreApplication::translate("InterfaceGrafica", "Limpar Todas Saídas") + ":";
        } catch (const std::exception& e) {
            LogManager::error(QString("Erro em retranslate (second quadrant): %1").arg(e.what()));
        }
    };

    if (self->gerenciador_traducao) {
        QObject::connect(self->gerenciador_traducao.get(), &GerenciadorTraducao::idioma_alterado, retranslate);
        retranslate();
    }

    return layout;
}
