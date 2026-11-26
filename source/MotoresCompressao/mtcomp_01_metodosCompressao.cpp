#include "mtcomp_01_metodosCompressao.hpp"
#include "source/GerenciamentoUI/ui_02_gerenteGUILayouts.hpp"
#include "source/utils/CaminhoPersistenteUtils.hpp"
#include "source/utils/LogManager.hpp"

#include <QCoreApplication>
#include <QCursor>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMouseEvent>
#include <QTimer>

MenuPersistente::MenuPersistente(const QString& titulo, QWidget* parent)
    : QMenu(titulo, parent)
{
    setAttribute(Qt::WA_DeleteOnClose, false);
    setMouseTracking(true);
}

void MenuPersistente::mousePressEvent(QMouseEvent* event) {
    QAction* action = actionAt(event->pos());
    if (action && action->isEnabled()) {
        if (!action->menu()) {
            QTimer::singleShot(0, action, [action]() {
                action->trigger();
            });
            event->accept();
            return;
        }
    }
    QMenu::mousePressEvent(event);
}

void MenuPersistente::leaveEvent(QEvent* event) {
    if (parentWidget()) {
        QPoint pos = mapFromGlobal(QCursor::pos());
        if (!rect().contains(pos)) {
            hide();
        }
    }
    QMenu::leaveEvent(event);
}

void MenuPersistente::mouseReleaseEvent(QMouseEvent* event) {
    QAction* action = actionAt(event->pos());
    if (action && action->isEnabled() && !action->menu()) {
        event->accept();
        return;
    }
    QMenu::mouseReleaseEvent(event);
}

QString compressTypeToString(CompressType type) {
    switch (type) {
    case CompressType::RAR: return "rar";
    case CompressType::ZIP: return "zip";
    case CompressType::SEVEN_Z: return "7z";
    case CompressType::TAR: return "tar";
    case CompressType::WIM: return "wim";
    }
    return "rar";
}

CompressType stringToCompressType(const QString& str) {
    if (str == "rar") return CompressType::RAR;
    if (str == "zip") return CompressType::ZIP;
    if (str == "7z") return CompressType::SEVEN_Z;
    if (str == "tar") return CompressType::TAR;
    if (str == "wim") return CompressType::WIM;
    return CompressType::RAR;
}

TranslatedLegends get_translated_legends() {
    TranslatedLegends tl;

    tl.rar["0"] = QString("0 - %1").arg(QCoreApplication::translate("CompressLevels", "Armazenar"));
    tl.rar["1"] = QString("1 - %1").arg(QCoreApplication::translate("CompressLevels", "Mais Rápido"));
    tl.rar["2"] = QString("2 - %1").arg(QCoreApplication::translate("CompressLevels", "Rápido"));
    tl.rar["3"] = QString("3 - %1").arg(QCoreApplication::translate("CompressLevels", "Normal"));
    tl.rar["4"] = QString("4 - %1").arg(QCoreApplication::translate("CompressLevels", "Bom"));
    tl.rar["5"] = QString("5 - %1").arg(QCoreApplication::translate("CompressLevels", "Ótimo"));

    tl.sevenz_zip["0"] = QString("0 - %1").arg(QCoreApplication::translate("CompressLevels", "Armazenar"));
    tl.sevenz_zip["1"] = QString("1 - %1").arg(QCoreApplication::translate("CompressLevels", "Mais Rápido"));
    tl.sevenz_zip["3"] = QString("3 - %1").arg(QCoreApplication::translate("CompressLevels", "Rápido"));
    tl.sevenz_zip["5"] = QString("5 - %1").arg(QCoreApplication::translate("CompressLevels", "Normal"));
    tl.sevenz_zip["7"] = QString("7 - %1").arg(QCoreApplication::translate("CompressLevels", "Máximo"));
    tl.sevenz_zip["9"] = QString("9 - %1").arg(QCoreApplication::translate("CompressLevels", "Ultra"));

    tl.tar["0"] = QString("0 - %1").arg(QCoreApplication::translate("CompressLevels", "Armazenar"));
    tl.tar_wim["0"] = QString("0 - %1").arg(QCoreApplication::translate("CompressLevels", "Armazenar"));

    tl.wim["0"] = QString("0 - %1").arg(QCoreApplication::translate("CompressLevels", "Armazenar"));

    return tl;
}

MetodoCompressao::MetodoCompressao() {
    config_menu = new QMenu();
    compression_method_action = new QAction();
}

void MetodoCompressao::select_compression_method() {
    QString titulo_metodo = QCoreApplication::translate("InterfaceGrafica", "Método de Compressão");
    if (!compression_menu) {
        compression_menu = new MenuPersistente(titulo_metodo, nullptr);
        if (config_menu) {
            compression_menu->setStyleSheet(config_menu->styleSheet());
        }
        if (compression_method_action) {
            compression_method_action->setMenu(compression_menu);
        }
    } else {
        compression_menu->clear();
        compression_menu->setTitle(titulo_metodo);
    }
    if (compression_method_action) {
        compression_method_action->setText(titulo_metodo);
    }

    method_actions.clear();
    TranslatedLegends legends = get_translated_legends();

#if ENABLE_RAR_SUPPORT
    add_submenu(compression_menu, "RAR (0-5)", legends.rar, CompressType::RAR);
#endif
    add_submenu(compression_menu, "ZIP (0-9)", legends.sevenz_zip, CompressType::ZIP);
    add_submenu(compression_menu, "7Z (0-9)", legends.sevenz_zip, CompressType::SEVEN_Z);
    add_submenu(compression_menu, "TAR (0)", legends.tar, CompressType::TAR);
    add_submenu(compression_menu, "WIM (0)", legends.wim, CompressType::WIM);
}

void MetodoCompressao::add_submenu(QMenu* parent_menu, const QString& title, const QMap<QString, QString>& legends, CompressType compress_type) {
    auto submenu = new MenuPersistente(title, parent_menu);
    if (config_menu) {
        submenu->setStyleSheet(config_menu->styleSheet());
    }

    QString compress_type_value = compressTypeToString(compress_type);
    QString current_method = get_current_method(compress_type_value);

    if (!method_actions.contains(compress_type_value)) {
        method_actions[compress_type_value] = QMap<QString, QAction*>();
    }

    for (auto it = legends.begin(); it != legends.end(); ++it) {
        QString method = it.key();
        QString legend = it.value();

        auto action = new QAction(legend, submenu);
        action->setCheckable(true);
        if (method == current_method) {
            action->setChecked(true);
        }

        QObject::connect(action, &QAction::triggered, [this, method, compress_type]() {
            this->set_compression_method(method, compress_type);
        });

        submenu->addAction(action);
        method_actions[compress_type_value][method] = action;
    }

    parent_menu->addMenu(submenu);
}

QString MetodoCompressao::get_current_method(const QString& compress_type_value) const {
    if (get_gerenciador_interface) {
        GerenciadorInterface* gi = get_gerenciador_interface();
        if (gi) {
            return gi->getCompressionMethod(compress_type_value);
        }
    }
    return QString();
}

void MetodoCompressao::set_compression_method(const QString& method, CompressType compress_type) {
    try {
        QString compress_type_value = compressTypeToString(compress_type);

        if (get_gerenciador_interface) {
            GerenciadorInterface* gi = get_gerenciador_interface();
            if (gi) {
                gi->setCompressionMethod(compress_type_value, method);
            }
        }

        if (get_gerenciador_interface_2) {
            GerenciadorInterface* gi2 = get_gerenciador_interface_2();
            if (gi2) {
                gi2->setCompressionMethod(compress_type_value, method);
            }
        }

        if (method_actions.contains(compress_type_value)) {
            for (auto it = method_actions[compress_type_value].begin(); it != method_actions[compress_type_value].end(); ++it) {
                it.value()->setChecked(it.key() == method);
            }
        }

        save_compression_method(get_config_path());
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao definir o método de compressão: %1").arg(e.what()));
    }
}

void MetodoCompressao::load_compression_method() {
    QString config_path = get_config_path();
    try {
        QFile file(config_path);
        if (file.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            file.close();
            if (doc.isObject()) {
                QJsonObject config = doc.object();
#if ENABLE_RAR_SUPPORT
                const CompressType types[] = { CompressType::RAR, CompressType::ZIP, CompressType::SEVEN_Z, CompressType::TAR, CompressType::WIM };
#else
                const CompressType types[] = { CompressType::ZIP, CompressType::SEVEN_Z, CompressType::TAR, CompressType::WIM };
#endif
                for (CompressType ct : types) {
                    QString val = compressTypeToString(ct);
                    QString key = "compress_type_" + val;
                    if (config.contains(key)) {
                        QString method = config.value(key).toString();
                        if (get_gerenciador_interface) {
                            GerenciadorInterface* gi = get_gerenciador_interface();
                            if (gi) gi->setCompressionMethod(val, method);
                        }
                        if (get_gerenciador_interface_2) {
                            GerenciadorInterface* gi2 = get_gerenciador_interface_2();
                            if (gi2) gi2->setCompressionMethod(val, method);
                        }
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao carregar método de compressão: %1").arg(e.what()));
    }
}

void MetodoCompressao::save_compression_method(const QString& config_path) {
    try {
        QDir().mkpath(QFileInfo(config_path).absolutePath());
        QJsonObject config;

#if ENABLE_RAR_SUPPORT
        const CompressType types[] = { CompressType::RAR, CompressType::ZIP, CompressType::SEVEN_Z, CompressType::TAR, CompressType::WIM };
#else
        const CompressType types[] = { CompressType::ZIP, CompressType::SEVEN_Z, CompressType::TAR, CompressType::WIM };
#endif
        for (CompressType ct : types) {
            QString val = compressTypeToString(ct);
            QString current = get_current_method(val);
            if (!current.isEmpty()) {
                config["compress_type_" + val] = current;
            }
        }

        QFile file(config_path);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(QJsonDocument(config).toJson());
            file.close();
        }
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao salvar método de compressão: %1").arg(e.what()));
    }
}

QString MetodoCompressao::get_config_path() const {
    return QDir(obter_caminho_persistente()).filePath("config.json");
}
