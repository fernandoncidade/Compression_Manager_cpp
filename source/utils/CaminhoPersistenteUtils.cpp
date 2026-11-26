#include "CaminhoPersistenteUtils.hpp"
#include "LogManager.hpp"
#include <QDir>
#include <QStandardPaths>

QString obter_caminho_persistente() {
    QString localAppData = qEnvironmentVariable("LOCALAPPDATA");
    if (localAppData.isEmpty()) {
        localAppData = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    }

    QDir configDir(QDir(localAppData).filePath("CompressionManager"));
    if (!configDir.exists()) {
        if (!configDir.mkpath(".")) {
            LogManager::error("Erro ao criar diretório de configuração persistente: " + configDir.absolutePath());
        }
    }
    return configDir.absolutePath();
}

QString obterCaminhoPersistente() {
    return obter_caminho_persistente();
}
