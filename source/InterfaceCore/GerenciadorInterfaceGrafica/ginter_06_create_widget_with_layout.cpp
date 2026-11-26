#include "ginter_06_create_widget_with_layout.hpp"
#include <QWidget>
#include <QLayout>

QWidget* create_widget_with_layout(InterfaceGrafica* self, QLayout* layout) {
    Q_UNUSED(self);
    auto widget = new QWidget();
    if (layout) {
        widget->setLayout(layout);
    }
    return widget;
}
