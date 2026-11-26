#include "ginter_11_create_main_layout_2.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include <QHBoxLayout>

QHBoxLayout* create_main_layout_2(InterfaceGrafica* self) {
    auto layout = new QHBoxLayout();
    if (self) {
        layout->addLayout(self->create_third_quadrant());
    }
    return layout;
}
