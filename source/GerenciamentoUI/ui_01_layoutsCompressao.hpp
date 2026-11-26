#pragma once

#include <QLayout>
#include <QWidget>
#include <QString>
#include <QMap>
#include <QList>
#include <QPointer>
#include <functional>

class GerenciadorInterface;
class QPushButton;

class LayoutsCompressao {
public:
    using CreateButtonFn = std::function<QPushButton*(const QString& text, std::function<void()> callback, const QString& icon)>;
    using LayoutCreatorFn = std::function<QLayout*()>;

    LayoutsCompressao(GerenciadorInterface* gerenciador_interface, CreateButtonFn create_button);
    virtual ~LayoutsCompressao() = default;

    QMap<QString, LayoutCreatorFn> create_compression_method_layouts();

    QLayout* create_layout(const QString& method, const QString& icon_name,
                           std::function<void()> store_callback,
                           std::function<void()> clear_callback);

    QLayout* create_rar_layout();
    QLayout* create_zip_layout();
    QLayout* create_7z_layout();
    QLayout* create_tar_layout();
    QLayout* create_wim_layout();
    QLayout* create_extracao_layout();

    void clear_left_widgets();

private:
    GerenciadorInterface* m_gerenciador_interface = nullptr;
    CreateButtonFn m_create_button;
    int m_max_left_width = 0;
    QList<QPointer<QWidget>> m_left_widgets;
};
