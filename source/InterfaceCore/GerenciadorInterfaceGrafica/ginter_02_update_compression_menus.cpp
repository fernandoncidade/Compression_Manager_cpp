#include "ginter_02_update_compression_menus.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"

void update_compression_menus(InterfaceGrafica* self) {
    if (self) {
        self->select_compression_method();
    }
}
