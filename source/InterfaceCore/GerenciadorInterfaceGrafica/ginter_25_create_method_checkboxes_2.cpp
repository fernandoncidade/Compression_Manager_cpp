#include "ginter_25_create_method_checkboxes_2.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/utils/LogManager.hpp"
#include <QHBoxLayout>
#include <QCheckBox>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QCoreApplication>

void ginter_create_method_checkboxes_2(InterfaceGrafica* self, QVBoxLayout* layout) {
    auto checkbox_layout = new QHBoxLayout();
    checkbox_layout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
#if ENABLE_RAR_SUPPORT
    QStringList methods = {"rar", "zip", "7z", "tar", "wim", "extracao"};
#else
    QStringList methods = {"zip", "7z", "tar", "wim", "extracao"};
#endif

    self->checkboxes_2.clear();

    for (const auto& method : methods) {
        QString display_name = (method == "extracao") 
            ? QCoreApplication::translate("InterfaceGrafica", "EXTRAÇÃO") 
            : method.toUpper();

        auto checkbox = new QCheckBox(display_name);
        checkbox->setProperty("method", method);
        QObject::connect(checkbox, &QCheckBox::toggled, [self](bool) {
            self->on_method_toggled_2();
        });
        checkbox_layout->addWidget(checkbox);
        self->checkboxes_2[method] = checkbox;
    }

    layout->addLayout(checkbox_layout);
}
