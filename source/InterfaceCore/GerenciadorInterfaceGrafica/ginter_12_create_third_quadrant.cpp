#include "ginter_12_create_third_quadrant.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/utils/LogManager.hpp"
#include <QVBoxLayout>

QVBoxLayout* ginter_create_third_quadrant(InterfaceGrafica* self) {
    auto layout = new QVBoxLayout();
    self->create_method_checkboxes(layout);
    return layout;
}
