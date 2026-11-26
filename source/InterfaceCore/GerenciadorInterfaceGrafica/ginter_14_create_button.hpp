#pragma once

#include <QString>
#include <functional>

class InterfaceGrafica;
class QPushButton;

QPushButton* ginter_create_button(InterfaceGrafica* self, const QString& text, std::function<void()> callback = nullptr, const QString& icon = QString());
