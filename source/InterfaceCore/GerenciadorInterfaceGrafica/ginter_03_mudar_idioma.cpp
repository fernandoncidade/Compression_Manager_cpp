#include "ginter_03_mudar_idioma.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"

void mudar_idioma(InterfaceGrafica* self, const QString& codigo_idioma) {
    if (!self) return;

    if (self->pt_br_action) {
        self->pt_br_action->setChecked(codigo_idioma == "pt_BR");
    }
    if (self->en_us_action) {
        self->en_us_action->setChecked(codigo_idioma == "en_US");
    }

    self->setProperty("idioma_atual", codigo_idioma);

    if (self->gerenciador_traducao) {
        self->gerenciador_traducao->definir_idioma(codigo_idioma);
    }
}
