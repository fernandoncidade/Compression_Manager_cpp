#include "ginter_13_create_scroll_area.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/utils/LogManager.hpp"
#include <QCoreApplication>
#include <QGroupBox>
#include <QScrollArea>
#include <QVBoxLayout>

QGroupBox* ginter_create_scroll_area(InterfaceGrafica* self) {
    self->methods_group = new QGroupBox(QCoreApplication::translate("InterfaceGrafica", "Métodos de compressão e extração"));
    self->methods_group->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto group_layout = new QVBoxLayout(self->methods_group);
    group_layout->setContentsMargins(8, 8, 8, 8);
    group_layout->setSpacing(6);

    self->scroll_area = new QScrollArea();
    self->scroll_area->setWidgetResizable(true);
    self->scroll_area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    self->scroll_area_widget = new QWidget();
    self->scroll_area_layout = new QVBoxLayout(self->scroll_area_widget);
    self->scroll_area_layout->setContentsMargins(6, 6, 6, 6);
    self->scroll_area_layout->setSpacing(6);
    self->scroll_area->setWidget(self->scroll_area_widget);

    self->scroll_area->setMinimumSize(250, 120);

    self->method_layouts_container = new QVBoxLayout();
    self->method_layouts_container->setContentsMargins(0, 0, 0, 0);
    self->method_layouts_container->setSpacing(6);
    self->scroll_area_layout->addLayout(self->method_layouts_container);

    group_layout->addWidget(self->scroll_area);

    return self->methods_group;
}
