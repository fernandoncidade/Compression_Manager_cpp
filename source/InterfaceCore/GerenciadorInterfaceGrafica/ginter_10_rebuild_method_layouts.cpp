#include "ginter_10_rebuild_method_layouts.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/GerenciamentoUI/ui_01_layoutsCompressao.hpp"
#include "source/utils/LogManager.hpp"

void rebuild_method_layouts(InterfaceGrafica* self) {
    if (!self) return;

    if (self->layouts_compressao) {
        self->layouts_compressao->clear_left_widgets();
    }
    if (self->layouts_compressao_2) {
        self->layouts_compressao_2->clear_left_widgets();
    }

    QStringList keys = self->current_layouts.keys();
    for (const QString& method : keys) {
        QLayout* layout_to_remove = self->current_layouts.take(method);
        self->remove_layout_widgets(layout_to_remove);
        if (self->method_layouts_container) {
            self->method_layouts_container->removeItem(layout_to_remove);
        }
        delete layout_to_remove;
    }

    for (auto it = self->checkboxes.begin(); it != self->checkboxes.end(); ++it) {
        if (it.value() && it.value()->isChecked()) {
            QString method = it.key();
            if (self->compression_method_layouts.contains(method)) {
                QLayout* new_layout = self->compression_method_layouts[method]();
                self->current_layouts[method] = new_layout;
                if (self->method_layouts_container) {
                    self->method_layouts_container->addLayout(new_layout);
                }
            }
        }
    }

    if (self->segundo_layout_action && self->segundo_layout_action->isChecked()) {
        QStringList keys2 = self->current_layouts_2.keys();
        for (const QString& method : keys2) {
            QLayout* layout_to_remove = self->current_layouts_2.take(method);
            self->remove_layout_widgets(layout_to_remove);
            if (self->method_layouts_container_2) {
                self->method_layouts_container_2->removeItem(layout_to_remove);
            }
            delete layout_to_remove;
        }

        for (auto it = self->checkboxes_2.begin(); it != self->checkboxes_2.end(); ++it) {
            if (it.value() && it.value()->isChecked()) {
                QString method = it.key();
                if (self->compression_method_layouts_2.contains(method)) {
                    QLayout* new_layout = self->compression_method_layouts_2[method]();
                    self->current_layouts_2[method] = new_layout;
                    if (self->method_layouts_container_2) {
                        self->method_layouts_container_2->addLayout(new_layout);
                    }
                }
            }
        }
    }

    self->adjust_scroll_area();
}
