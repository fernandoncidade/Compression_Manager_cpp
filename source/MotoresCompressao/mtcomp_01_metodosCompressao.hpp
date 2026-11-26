#pragma once

#include <QMenu>
#include <QAction>
#include <QMap>
#include <QString>
#include <functional>

class GerenciadorInterface;

class MenuPersistente : public QMenu {
    Q_OBJECT

public:
    explicit MenuPersistente(const QString& titulo, QWidget* parent = nullptr);
    ~MenuPersistente() override = default;

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};

enum class CompressType {
    RAR,
    ZIP,
    SEVEN_Z,
    TAR,
    WIM
};

QString compressTypeToString(CompressType type);
CompressType stringToCompressType(const QString& str);

struct TranslatedLegends {
    QMap<QString, QString> rar;
    QMap<QString, QString> sevenz_zip;
    QMap<QString, QString> tar;
    QMap<QString, QString> wim;
    QMap<QString, QString> tar_wim;
};

TranslatedLegends get_translated_legends();

class MetodoCompressao {
public:
    MetodoCompressao();
    virtual ~MetodoCompressao() = default;

    void select_compression_method();
    void add_submenu(QMenu* parent_menu, const QString& title, const QMap<QString, QString>& legends, CompressType compress_type);
    QString get_current_method(const QString& compress_type_value) const;
    void set_compression_method(const QString& method, CompressType compress_type);
    void load_compression_method();
    void save_compression_method(const QString& config_path);
    QString get_config_path() const;

    QMenu* config_menu = nullptr;
    QAction* compression_method_action = nullptr;
    MenuPersistente* compression_menu = nullptr;
    QMap<QString, QMap<QString, QAction*>> method_actions;

    // Callbacks or references to target interface managers
    std::function<GerenciadorInterface*()> get_gerenciador_interface;
    std::function<GerenciadorInterface*()> get_gerenciador_interface_2;
};
