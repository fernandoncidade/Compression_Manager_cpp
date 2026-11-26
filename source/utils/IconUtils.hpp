#pragma once

#include <QString>

QString get_icon_path(const QString& icon_name);
QString getIconPath(const QString& icon_name);

namespace IconUtils {
    inline QString get_icon_path(const QString& icon_name) { return ::get_icon_path(icon_name); }
    inline QString getIconPath(const QString& icon_name) { return ::getIconPath(icon_name); }
}
