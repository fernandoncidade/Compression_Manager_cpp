#include "ginter_23_create_first_quadrant_2.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/utils/LogManager.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QCoreApplication>

QVBoxLayout* ginter_create_first_quadrant_2(InterfaceGrafica* self) {
    auto layout = new QVBoxLayout();
    layout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    self->add_folders_label_2 = new QLabel(QCoreApplication::translate("InterfaceGrafica", "Adicionar Pastas") + ":");
    auto add_folders_button_2 = self->create_button("", [self]() {
        self->gerenciador_interface_2->browse_folder();
    }, "pasta.png");
    auto row1 = new QHBoxLayout();
    row1->addWidget(self->add_folders_label_2);
    row1->addWidget(add_folders_button_2);
    self->main_buttons_2[self->add_folders_label_2] = "Adicionar Pastas:";

    self->add_files_label_2 = new QLabel(QCoreApplication::translate("InterfaceGrafica", "Adicionar Arquivos") + ":");
    auto add_files_button_2 = self->create_button("", [self]() {
        self->gerenciador_interface_2->browse_file();
    }, "arquivo.png");
    auto row2 = new QHBoxLayout();
    row2->addWidget(self->add_files_label_2);
    row2->addWidget(add_files_button_2);
    self->main_buttons_2[self->add_files_label_2] = "Adicionar Arquivos:";

    self->test_label_2 = new QLabel(QCoreApplication::translate("InterfaceGrafica", "Testar Integridade") + ":");
    auto test_integrity_button_2 = self->create_button("", [self]() {
        self->gerenciador_interface_2->testar_integridade();
    }, "interrogacao.png");
    auto row3 = new QHBoxLayout();
    row3->addWidget(self->test_label_2);
    row3->addWidget(test_integrity_button_2);
    self->main_buttons_2[self->test_label_2] = "Testar Integridade:";

    layout->addLayout(row1);
    layout->addLayout(row2);
    layout->addLayout(row3);

    auto retranslate = [self]() {
        try {
            if (self->add_folders_label_2) self->add_folders_label_2->setText(QCoreApplication::translate("InterfaceGrafica", "Adicionar Pastas") + ":");
            if (self->add_files_label_2) self->add_files_label_2->setText(QCoreApplication::translate("InterfaceGrafica", "Adicionar Arquivos") + ":");
            if (self->test_label_2) self->test_label_2->setText(QCoreApplication::translate("InterfaceGrafica", "Testar Integridade") + ":");

            if (self->add_folders_label_2) self->main_buttons_2[self->add_folders_label_2] = QCoreApplication::translate("InterfaceGrafica", "Adicionar Pastas") + ":";
            if (self->add_files_label_2) self->main_buttons_2[self->add_files_label_2] = QCoreApplication::translate("InterfaceGrafica", "Adicionar Arquivos") + ":";
            if (self->test_label_2) self->main_buttons_2[self->test_label_2] = QCoreApplication::translate("InterfaceGrafica", "Testar Integridade") + ":";
        } catch (const std::exception& e) {
            LogManager::instance().log(QString("Erro em retranslate (first quadrant 2): %1").arg(e.what()), "ERROR");
        }
    };

    if (self->gerenciador_traducao && self->add_folders_label_2) {
        QObject::connect(self->gerenciador_traducao.get(), &GerenciadorTraducao::idioma_alterado, self->add_folders_label_2, retranslate);
        retranslate();
    }

    return layout;
}
