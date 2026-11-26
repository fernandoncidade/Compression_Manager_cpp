#include "tr_01_gerenciadorTraducao.hpp"
#include "source/utils/CaminhoPersistenteUtils.hpp"
#include "source/utils/ApplicationPathUtils.hpp"
#include "source/utils/LogManager.hpp"

#include <QCoreApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QPushButton>

GerenciadorTraducao::GerenciadorTraducao(QObject* parent)
    : QObject(parent),
      idioma_atual("pt_BR"),
      idioma_padrao("pt_BR")
{
    idiomas_disponiveis["pt_BR"] = "Português (Brasil)";
    idiomas_disponiveis["en_US"] = "English (United States)";

    QString basePath = get_app_base_path();
    QString appDir = QCoreApplication::applicationDirPath();

    QStringList candidates = {
        QDir(appDir).filePath("translations"),
        QDir(basePath).filePath("translations"),
        QDir(basePath).filePath("source/language/translations"),
        QDir(appDir).filePath("language/translations"),
        QDir(appDir).filePath("../source/language/translations")
    };

    dir_traducoes = candidates.first();
    for (const QString& c : candidates) {
        if (QDir(c).exists()) {
            dir_traducoes = QDir(c).absolutePath();
            break;
        }
    }

    QDir().mkpath(dir_traducoes);
    carregar_configuracao_idioma();
}

GerenciadorTraducao::~GerenciadorTraducao() {
    if (tradutor && QCoreApplication::instance()) {
        QCoreApplication::removeTranslator(tradutor.get());
    }
}

QString GerenciadorTraducao::obter_caminho_configuracao() {
    try {
        QString persistDir = obter_caminho_persistente();
        return QDir(persistDir).filePath("language.json");
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao obter caminho de configuração de idioma: %1").arg(e.what()));
        return QString();
    }
}

void GerenciadorTraducao::carregar_configuracao_idioma() {
    QString configPath = obter_caminho_configuracao();
    try {
        if (!configPath.isEmpty() && QFile::exists(configPath)) {
            QFile file(configPath);
            if (file.open(QIODevice::ReadOnly)) {
                QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
                file.close();
                if (doc.isObject()) {
                    QJsonObject obj = doc.object();
                    if (obj.contains("idioma")) {
                        idioma_atual = obj.value("idioma").toString("pt_BR");
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao carregar configuração de idioma: %1").arg(e.what()));
    }
}

void GerenciadorTraducao::salvar_configuracao_idioma() {
    QString configPath = obter_caminho_configuracao();
    try {
        if (!configPath.isEmpty()) {
            QDir().mkpath(QFileInfo(configPath).absolutePath());
            QFile file(configPath);
            if (file.open(QIODevice::WriteOnly)) {
                QJsonObject obj;
                obj["idioma"] = idioma_atual;
                file.write(QJsonDocument(obj).toJson());
                file.close();
            }
        }
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao salvar configuração de idioma: %1").arg(e.what()));
    }
}

bool GerenciadorTraducao::aplicar_traducao() {
    try {
        if (tradutor) {
            QCoreApplication::removeTranslator(tradutor.get());
            tradutor.reset();
        }

        if (idioma_atual == idioma_padrao) {
            return true;
        }

        tradutor = std::make_unique<QTranslator>();
        QString qmFile = QString("manager_compression_%1.qm").arg(idioma_atual);
        QString translationPath = QDir(dir_traducoes).filePath(qmFile);

        if (!QFile::exists(translationPath)) {
            QString altBase = get_app_base_path();
            QString altPath = QDir(altBase).filePath(QString("source/language/translations/%1").arg(qmFile));
            if (QFile::exists(altPath)) {
                translationPath = altPath;
            }
        }

        if (QFile::exists(translationPath)) {
            if (tradutor->load(translationPath)) {
                QCoreApplication::installTranslator(tradutor.get());
                LogManager::info("Tradução carregada com sucesso: " + translationPath);
                return true;
            } else {
                LogManager::error("Erro ao carregar arquivo de tradução: " + translationPath);
            }
        } else {
            LogManager::warning("Arquivo de tradução não encontrado: " + translationPath);
        }
        return false;
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao aplicar tradução: %1").arg(e.what()));
        return false;
    }
}

bool GerenciadorTraducao::definir_idioma(const QString& codigo_idioma) {
    try {
        if (idiomas_disponiveis.contains(codigo_idioma)) {
            idioma_atual = codigo_idioma;
            salvar_configuracao_idioma();
            bool ok = aplicar_traducao();
            emit idioma_alterado(codigo_idioma);
            return ok;
        }
        return false;
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao definir idioma: %1").arg(e.what()));
        return false;
    }
}

QString GerenciadorTraducao::obter_idioma_atual() const {
    return idioma_atual;
}

void GerenciadorTraducao::traduzir_botoes_padrao(QDialog* dialogo) {
    if (!dialogo) return;
    try {
        auto mbox = qobject_cast<QMessageBox*>(dialogo);
        if (mbox) {
            struct BtnMapping {
                QMessageBox::StandardButton btn;
                const char* text;
            };
            const BtnMapping mappings[] = {
                { QMessageBox::Ok, "OK" },
                { QMessageBox::Cancel, "Cancelar" },
                { QMessageBox::Yes, "Sim" },
                { QMessageBox::No, "Não" },
                { QMessageBox::Abort, "Abortar" },
                { QMessageBox::Retry, "Tentar Novamente" },
                { QMessageBox::Ignore, "Ignorar" },
                { QMessageBox::Close, "Fechar" },
                { QMessageBox::Help, "Ajuda" },
                { QMessageBox::Apply, "Aplicar" },
                { QMessageBox::Reset, "Redefinir" },
                { QMessageBox::RestoreDefaults, "Restaurar Padrões" },
                { QMessageBox::Save, "Salvar" },
                { QMessageBox::SaveAll, "Salvar Tudo" },
                { QMessageBox::Open, "Abrir" },
            };

            for (const auto& item : mappings) {
                QAbstractButton* b = mbox->button(item.btn);
                if (b) {
                    b->setText(QCoreApplication::translate("Dialog", item.text));
                }
            }
        }
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao traduzir botões padrão: %1").arg(e.what()));
    }
}
