#include "ginter_26_on_method_toggled_2.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/utils/LogManager.hpp"
#include <QCheckBox>
#include <QLayout>

void ginter_on_method_toggled_2(InterfaceGrafica* self) {
    for (auto it = self->checkboxes_2.begin(); it != self->checkboxes_2.end(); ++it) {
        const QString& method = it.key();
        QCheckBox* checkbox = it.value();

        if (checkbox->isChecked() && !self->current_layouts_2.contains(method)) {
            if (self->compression_method_layouts_2.contains(method)) {
                QLayout* new_layout = self->compression_method_layouts_2[method]();
                self->current_layouts_2[method] = new_layout;
                self->method_layouts_container_2->addLayout(new_layout);
            }
        } else if (!checkbox->isChecked() && self->current_layouts_2.contains(method)) {
            QLayout* layout_to_remove = self->current_layouts_2.take(method);
            self->remove_layout_widgets(layout_to_remove);
            self->method_layouts_container_2->removeItem(layout_to_remove);
            layout_to_remove->deleteLater();
        }
    }

    self->adjust_scroll_area();
}
