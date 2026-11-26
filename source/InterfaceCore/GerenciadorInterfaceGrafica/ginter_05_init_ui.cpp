#include "ginter_05_init_ui.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/utils/IconUtils.hpp"
#include "source/utils/LogManager.hpp"
#include <QCoreApplication>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QSizePolicy>

void init_ui(InterfaceGrafica* self) {
    self->setWindowTitle(QCoreApplication::translate("InterfaceGrafica", "Compression Manager"));
    QString icon_path = get_icon_path("Manager-BackUp.ico");
    if (!icon_path.isEmpty()) {
        self->setWindowIcon(QIcon(icon_path));
    }

    self->setMinimumSize(700, 520);
    self->resize(700, 520);

    self->main_buttons.clear();
    self->main_buttons_2.clear();
    self->folder_label = nullptr;
    self->folder_label_2 = nullptr;

    self->primeiro_layout_column = new QVBoxLayout();
    self->main_layout_1_widget = self->create_widget_with_layout(self->create_main_layout_1());
    self->main_layout_1_widget->setMinimumHeight(152);
    self->main_layout_1_widget->setMaximumHeight(304);
    self->primeiro_layout_column->addWidget(self->main_layout_1_widget, 0);

    auto main_layout_2_widget = self->create_widget_with_layout(self->create_main_layout_2());
    main_layout_2_widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    self->primeiro_layout_column->addWidget(main_layout_2_widget, 0);

    auto scroll_group = self->create_scroll_area();
    scroll_group->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    self->primeiro_layout_column->addWidget(scroll_group, 1);

    self->primeiro_layout_widget = new QWidget();
    self->primeiro_layout_widget->setLayout(self->primeiro_layout_column);
    self->primeiro_layout_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    self->segundo_layout_widget = nullptr;

    self->columns_widget = new QWidget(self);
    auto columns_layout = new QHBoxLayout(self->columns_widget);
    columns_layout->setContentsMargins(0, 0, 0, 0);
    columns_layout->setSpacing(10);
    columns_layout->addWidget(self->primeiro_layout_widget, 1);

    auto main_layout = new QVBoxLayout();
    main_layout->addWidget(self->columns_widget);

    auto central_widget = new QWidget(self);
    central_widget->setLayout(main_layout);
    self->setCentralWidget(central_widget);

    self->adjust_scroll_area();
}
