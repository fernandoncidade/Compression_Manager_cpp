#include "ginter_08_create_first_quadrant.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/utils/LogManager.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QCoreApplication>

QVBoxLayout* create_first_quadrant(InterfaceGrafica* self) {
    auto layout = new QVBoxLayout();
    layout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    self->add_folders_label = new QLabel(QCoreApplication::translate("InterfaceGrafica", "Adicionar Pastas") + ":");
    auto add_folders_button = self->create_button("", [self]() {
        self->gerenciador_interface->browse_folder();
    }, "pasta.png");
    auto row1 = new QHBoxLayout();
    row1->addWidget(self->add_folders_label);
    row1->addWidget(add_folders_button);
    self->main_buttons[self->add_folders_label] = "Adicionar Pastas:";

    self->add_files_label = new QLabel(QCoreApplication::translate("InterfaceGrafica", "Adicionar Arquivos") + ":");
    auto add_files_button = self->create_button("", [self]() {
        self->gerenciador_interface->browse_file();
    }, "arquivo.png");
    auto row2 = new QHBoxLayout();
    row2->addWidget(self->add_files_label);
    row2->addWidget(add_files_button);
    self->main_buttons[self->add_files_label] = "Adicionar Arquivos:";

    self->test_label = new QLabel(QCoreApplication::translate("InterfaceGrafica", "Testar Integridade") + ":");
    auto test_integrity_button = self->create_button("", [self]() {
        self->gerenciador_interface->testar_integridade();
    }, "interrogacao.png");
    auto row3 = new QHBoxLayout();
    row3->addWidget(self->test_label);
    row3->addWidget(test_integrity_button);
    self->main_buttons[self->test_label] = "Testar Integridade:";

    layout->addLayout(row1);
    layout->addLayout(row2);
    layout->addLayout(row3);

    auto retranslate = [self]() {
        try {
            if (self->add_folders_label) self->add_folders_label->setText(QCoreApplication::translate("InterfaceGrafica", "Adicionar Pastas") + ":");
            if (self->add_files_label) self->add_files_label->setText(QCoreApplication::translate("InterfaceGrafica", "Adicionar Arquivos") + ":");
            if (self->test_label) self->test_label->setText(QCoreApplication::translate("InterfaceGrafica", "Testar Integridade") + ":");

            if (self->add_folders_label) self->main_buttons[self->add_folders_label] = QCoreApplication::translate("InterfaceGrafica", "Adicionar Pastas") + ":";
            if (self->add_files_label) self->main_buttons[self->add_files_label] = QCoreApplication::translate("InterfaceGrafica", "Adicionar Arquivos") + ":";
            if (self->test_label) self->main_buttons[self->test_label] = QCoreApplication::translate("InterfaceGrafica", "Testar Integridade") + ":";
        } catch (const std::exception& e) {
            LogManager::error(QString("Erro em retranslate (first quadrant): %1").arg(e.what()));
        }
    };

    if (self->gerenciador_traducao) {
        QObject::connect(self->gerenciador_traducao.get(), &GerenciadorTraducao::idioma_alterado, retranslate);
        retranslate();
    }

    return layout;
}
