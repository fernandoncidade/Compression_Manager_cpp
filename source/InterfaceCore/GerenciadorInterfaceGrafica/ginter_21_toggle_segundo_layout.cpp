#include "ginter_21_toggle_segundo_layout.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/GerenciamentoUI/ui_01_layoutsCompressao.hpp"
#include "source/utils/LogManager.hpp"
#include <algorithm>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QSizePolicy>

void ginter_toggle_segundo_layout(InterfaceGrafica* self) {
    bool is_checked = self->segundo_layout_action && self->segundo_layout_action->isChecked();

    if (is_checked) {
        if (!self->isMaximized() && self->width() < 1200) {
            self->resize(1200, std::max(self->height(), 750));
        }

        self->segundo_layout_column = new QVBoxLayout();

        auto first_quadrant_2 = self->create_first_quadrant_2();
        auto second_quadrant_2 = self->create_second_quadrant_2();
        auto main_layout_1_2 = new QHBoxLayout();
        main_layout_1_2->addLayout(first_quadrant_2);
        main_layout_1_2->addLayout(second_quadrant_2);

        self->main_layout_1_widget_2 = self->create_widget_with_layout(main_layout_1_2);
        self->main_layout_1_widget_2->setMinimumHeight(152);
        self->main_layout_1_widget_2->setMaximumHeight(304);
        self->segundo_layout_column->addWidget(self->main_layout_1_widget_2, 0);

        auto third_quadrant_2 = new QVBoxLayout();
        self->create_method_checkboxes_2(third_quadrant_2);
        auto main_layout_2_2 = new QHBoxLayout();
        main_layout_2_2->addLayout(third_quadrant_2);
        auto main_layout_2_widget_2 = self->create_widget_with_layout(main_layout_2_2);
        main_layout_2_widget_2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        self->segundo_layout_column->addWidget(main_layout_2_widget_2, 0);

        auto scroll_area_2_widget = self->create_scroll_area_2();
        scroll_area_2_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        self->segundo_layout_column->addWidget(scroll_area_2_widget, 1);

        self->segundo_layout_widget = new QWidget();
        self->segundo_layout_widget->setLayout(self->segundo_layout_column);
        self->segundo_layout_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        if (self->columns_widget && self->columns_widget->layout()) {
            auto box = qobject_cast<QBoxLayout*>(self->columns_widget->layout());
            if (box) {
                box->addWidget(self->segundo_layout_widget, 1);
            }
        }

        self->adjust_scroll_area();
        self->adapt_layout_orientation();
    } else {
        auto keys = self->current_layouts_2.keys();
        for (const auto& method : keys) {
            QLayout* layout_to_remove = self->current_layouts_2.take(method);
            self->remove_layout_widgets(layout_to_remove);
            if (self->method_layouts_container_2) {
                self->method_layouts_container_2->removeItem(layout_to_remove);
            }
            delete layout_to_remove;
        }

        if (self->segundo_layout_widget) {
            if (self->columns_widget && self->columns_widget->layout()) {
                self->columns_widget->layout()->removeWidget(self->segundo_layout_widget);
            }
            delete self->segundo_layout_widget;
            self->segundo_layout_widget = nullptr;
        }

        self->segundo_layout_column = nullptr;
        self->main_layout_1_widget_2 = nullptr;
        self->methods_group_2 = nullptr;
        self->scroll_area_2 = nullptr;
        self->scroll_area_widget_2 = nullptr;
        self->scroll_area_layout_2 = nullptr;
        self->method_layouts_container_2 = nullptr;

        self->folder_label_2 = nullptr;
        self->add_folders_label_2 = nullptr;
        self->add_files_label_2 = nullptr;
        self->test_label_2 = nullptr;
        self->clear_input_label_2 = nullptr;
        self->clear_outputs_label_2 = nullptr;

        if (self->gerenciador_interface_2) {
            self->gerenciador_interface_2->folder_listbox = nullptr;
        }

        self->checkboxes_2.clear();
        self->main_buttons_2.clear();
        if (self->layouts_compressao_2) {
            self->layouts_compressao_2->clear_left_widgets();
        }

        self->adjust_scroll_area();
        self->adapt_layout_orientation();
        if (!self->isMaximized() && self->width() >= 1200) {
            self->resize(700, 520);
        }
    }
}
