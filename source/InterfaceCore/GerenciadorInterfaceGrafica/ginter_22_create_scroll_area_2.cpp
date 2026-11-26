#include "ginter_22_create_scroll_area_2.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/utils/LogManager.hpp"
#include <QCoreApplication>
#include <QGroupBox>
#include <QScrollArea>
#include <QVBoxLayout>

QGroupBox* ginter_create_scroll_area_2(InterfaceGrafica* self) {
    self->methods_group_2 = new QGroupBox(QCoreApplication::translate("InterfaceGrafica", "Métodos de compressão e extração"));
    self->methods_group_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto group_layout = new QVBoxLayout(self->methods_group_2);
    group_layout->setContentsMargins(8, 8, 8, 8);
    group_layout->setSpacing(6);

    self->scroll_area_2 = new QScrollArea();
    self->scroll_area_2->setWidgetResizable(true);
    self->scroll_area_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    self->scroll_area_widget_2 = new QWidget();
    self->scroll_area_layout_2 = new QVBoxLayout(self->scroll_area_widget_2);
    self->scroll_area_layout_2->setContentsMargins(6, 6, 6, 6);
    self->scroll_area_layout_2->setSpacing(6);
    self->scroll_area_2->setWidget(self->scroll_area_widget_2);

    self->scroll_area_2->setMinimumSize(250, 120);

    self->method_layouts_container_2 = new QVBoxLayout();
    self->method_layouts_container_2->setContentsMargins(0, 0, 0, 0);
    self->method_layouts_container_2->setSpacing(6);
    self->scroll_area_layout_2->addLayout(self->method_layouts_container_2);

    group_layout->addWidget(self->scroll_area_2);

    return self->methods_group_2;
}
