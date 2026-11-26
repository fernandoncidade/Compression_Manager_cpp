#include "ginter_07_create_main_layout_1.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include <QHBoxLayout>

QHBoxLayout* create_main_layout_1(InterfaceGrafica* self) {
    auto layout = new QHBoxLayout();
    if (self) {
        layout->addLayout(self->create_first_quadrant());
        layout->addLayout(self->create_second_quadrant());
    }
    return layout;
}
