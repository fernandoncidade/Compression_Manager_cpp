#include "IconUtils.hpp"
#include "ApplicationPathUtils.hpp"
#include "LogManager.hpp"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QStringList>

QString get_icon_path(const QString& icon_name) {
    try {
        QString basePath = get_app_base_path();
        QString appDir = QCoreApplication::applicationDirPath();
        QString currentDir = QDir::currentPath();

        QStringList baseRoots = {
            basePath,
            QDir(basePath).filePath("source"),
            QDir(basePath).filePath(".."),
            appDir,
            QDir(appDir).filePath(".."),
            currentDir,
            QDir(currentDir).filePath("source"),
            QDir(currentDir).filePath("..")
        };

        QStringList candidatePaths;
        for (const QString& root : baseRoots) {
            candidatePaths << QDir(root).filePath(QString("assets/icones/%1").arg(icon_name));
            candidatePaths << QDir(root).filePath(QString("icones/%1").arg(icon_name));
            candidatePaths << QDir(root).filePath(QString("_internal/assets/icones/%1").arg(icon_name));
            candidatePaths << QDir(root).filePath(QString("_internal/icones/%1").arg(icon_name));
            candidatePaths << QDir(root).filePath(QString("mocks/icones/%1").arg(icon_name));
        }

        for (const QString& p : candidatePaths) {
            QFileInfo fi(p);
            if (fi.exists() && fi.isFile()) {
                return fi.absoluteFilePath();
            }
        }

        return QFileInfo(candidatePaths.value(0)).absoluteFilePath();
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao obter caminho do ícone '%1': %2").arg(icon_name, e.what()));
        return QString();
    }
}

QString getIconPath(const QString& icon_name) {
    return get_icon_path(icon_name);
}
