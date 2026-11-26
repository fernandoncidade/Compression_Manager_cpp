#include "ginter_14_create_button.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/utils/IconUtils.hpp"
#include "source/utils/LogManager.hpp"
#include <QPushButton>
#include <QFont>
#include <QFontMetrics>
#include <QIcon>

QPushButton* ginter_create_button(InterfaceGrafica* self, const QString& text, std::function<void()> callback, const QString& icon) {
    (void)self;
    auto button = new QPushButton(text);
    QFontMetrics fm(button->fontMetrics());
    int m_advance = fm.horizontalAdvance('m');
    button->setMinimumWidth(3 * m_advance);
    button->setMaximumWidth(3 * m_advance);
    int fm_height = static_cast<int>(1.5 * fm.height());
    button->setMinimumHeight(fm_height);
    button->setMaximumHeight(fm_height);
    button->setFont(QFont("Arial", 9));

    if (!icon.isEmpty()) {
        QString icon_path = IconUtils::get_icon_path(icon);
        if (!icon_path.isEmpty()) {
            button->setIcon(QIcon(icon_path));
        }
    }

    if (callback) {
        QObject::connect(button, &QPushButton::clicked, [callback]() {
            callback();
        });
    }

    return button;
}
