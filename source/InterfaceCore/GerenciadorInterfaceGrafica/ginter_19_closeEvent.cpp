#include "ginter_19_closeEvent.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/GerenciamentoUI/ui_07_Manual.hpp"
#include "source/GerenciamentoUI/ui_06_SobreDialog.hpp"
#include "source/utils/LogManager.hpp"
#include <QCloseEvent>
#include <QCoreApplication>

void ginter_closeEvent(InterfaceGrafica* self, QCloseEvent* event) {
    if (self->dialog_manual) {
        self->dialog_manual->close();
    }
    if (self->dialog_sobre) {
        self->dialog_sobre->close();
    }
    if (self->gerenciador_traducao && self->gerenciador_traducao->obter_tradutor()) {
        QCoreApplication::removeTranslator(self->gerenciador_traducao->obter_tradutor());
    }
    event->accept();
}
