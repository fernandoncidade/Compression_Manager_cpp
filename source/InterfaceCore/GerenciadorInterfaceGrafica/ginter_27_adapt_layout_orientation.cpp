#include "ginter_27_adapt_layout_orientation.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/utils/LogManager.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

void ginter_adapt_layout_orientation(InterfaceGrafica* self) {
    if (!self->columns_widget || !self->primeiro_layout_widget) {
        return;
    }

    bool is_segundo_active = (self->segundo_layout_action && 
                              self->segundo_layout_action->isChecked() && 
                              self->segundo_layout_widget != nullptr);

    int win_w = self->width();
    int win_h = self->height();

    bool is_portrait = is_segundo_active && (win_h > 1.5 * win_w);

    QLayout* current_layout = self->columns_widget->layout();
    if (current_layout) {
        bool current_is_vbox = (qobject_cast<QVBoxLayout*>(current_layout) != nullptr);
        if (current_is_vbox == is_portrait) {
            return;
        }

        current_layout->removeWidget(self->primeiro_layout_widget);
        if (is_segundo_active && self->segundo_layout_widget) {
            current_layout->removeWidget(self->segundo_layout_widget);
        }
        delete current_layout;
    }

    QBoxLayout* new_layout = nullptr;
    if (is_portrait) {
        new_layout = new QVBoxLayout();
    } else {
        new_layout = new QHBoxLayout();
    }

    new_layout->setContentsMargins(0, 0, 0, 0);
    new_layout->setSpacing(10);
    new_layout->addWidget(self->primeiro_layout_widget, 1);

    if (is_segundo_active && self->segundo_layout_widget) {
        new_layout->addWidget(self->segundo_layout_widget, 1);
    }

    self->columns_widget->setLayout(new_layout);
}
