#include "ginter_18_remove_layout_widgets.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/GerenciamentoUI/ui_03_dragDrop.hpp"
#include "source/utils/LogManager.hpp"
#include <QLayout>
#include <QWidget>

void ginter_remove_layout_widgets(InterfaceGrafica* self, QLayout* layout) {
    if (!layout) {
        return;
    }
    while (layout->count() > 0) {
        QLayoutItem* item = layout->takeAt(0);
        if (!item) continue;
        QWidget* widget = item->widget();
        if (widget) {
            if (qobject_cast<DragDropListWidget*>(widget)) {
                widget->setParent(nullptr);
            } else {
                widget->setParent(nullptr);
                widget->deleteLater();
            }
        } else if (item->layout()) {
            ginter_remove_layout_widgets(self, item->layout());
        }
        delete item;
    }
}
