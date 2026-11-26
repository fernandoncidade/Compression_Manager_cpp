#include "ginter_16_on_method_toggled.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/utils/LogManager.hpp"
#include <QCheckBox>
#include <QLayout>

void ginter_on_method_toggled(InterfaceGrafica* self) {
    for (auto it = self->checkboxes.begin(); it != self->checkboxes.end(); ++it) {
        const QString& method = it.key();
        QCheckBox* checkbox = it.value();

        if (checkbox->isChecked() && !self->current_layouts.contains(method)) {
            if (self->compression_method_layouts.contains(method)) {
                QLayout* new_layout = self->compression_method_layouts[method]();
                self->current_layouts[method] = new_layout;
                self->method_layouts_container->addLayout(new_layout);
            }
        } else if (!checkbox->isChecked() && self->current_layouts.contains(method)) {
            QLayout* layout_to_remove = self->current_layouts.take(method);
            self->remove_layout_widgets(layout_to_remove);
            self->method_layouts_container->removeItem(layout_to_remove);
            layout_to_remove->deleteLater();
        }
    }

    self->adjust_scroll_area();
}
