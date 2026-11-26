#include "ApplicationPathUtils.hpp"
#include "LogManager.hpp"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStringList>

QString get_app_base_path() {
    QString appDir = QCoreApplication::applicationDirPath();
    QString currentDir = QDir::currentPath();

    // Candidate root folders
    QStringList candidates = {
        appDir,
        QDir(appDir).filePath(".."),
        QDir(appDir).filePath("../source"),
        QDir(appDir).filePath("source"),
        currentDir,
        QDir(currentDir).filePath("source"),
        QDir(currentDir).filePath(".."),
        QDir(currentDir).filePath("../source"),
        "C:\\Program Files\\Compression Manager",
        "C:\\Program Files (x86)\\Compression Manager"
    };

    for (const QString& candidate : candidates) {
        QDir dir(candidate);
        if (dir.exists("assets") || dir.exists("source/assets") || dir.exists("plugins")) {
            return dir.absolutePath();
        }
    }

    return QDir(appDir).absolutePath();
}

QString getAppBasePath() {
    return get_app_base_path();
}

QString get_text_file_path(const QString& filename, const QString& folder) {
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
            "C:\\Program Files\\Compression Manager",
            "C:\\Program Files (x86)\\Compression Manager"
        };

        QStringList possiblePaths;
        for (const QString& root : baseRoots) {
            if (!folder.isEmpty()) {
                possiblePaths << QDir(root).filePath(QString("assets/%1/%2").arg(folder, filename));
                possiblePaths << QDir(root).filePath(QString("%1/%2").arg(folder, filename));
                possiblePaths << QDir(root).filePath(QString("_internal/assets/%1/%2").arg(folder, filename));
                possiblePaths << QDir(root).filePath(QString("_internal/%1/%2").arg(folder, filename));
            } else {
                possiblePaths << QDir(root).filePath(QString("assets/%1").arg(filename));
                possiblePaths << QDir(root).filePath(filename);
                possiblePaths << QDir(root).filePath(QString("_internal/assets/%1").arg(filename));
                possiblePaths << QDir(root).filePath(QString("_internal/%1").arg(filename));
            }
        }

        for (const QString& p : possiblePaths) {
            QFileInfo fi(p);
            if (fi.exists() && fi.isFile()) {
                return fi.absoluteFilePath();
            }
        }

        return QFileInfo(possiblePaths.value(0)).absoluteFilePath();
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao obter caminho do arquivo de texto '%1': %2").arg(filename, e.what()));
        return QString();
    }
}

QString getTextFilePath(const QString& filename, const QString& folder) {
    return get_text_file_path(filename, folder);
}

QString load_text_file(const QString& filename, const QString& folder, const QString& encoding) {
    Q_UNUSED(encoding);
    try {
        QString filePath = get_text_file_path(filename, folder);
        if (filePath.isEmpty() || !QFile::exists(filePath)) {
            LogManager::error(QString("Arquivo de texto '%1' não encontrado.").arg(filename));
            return QString();
        }

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            LogManager::error(QString("Erro ao abrir arquivo de texto '%1'.").arg(filename));
            return QString();
        }

        QByteArray bytes = file.readAll();
        file.close();

        // Handle UTF-8 BOM if present
        if (bytes.startsWith("\xEF\xBB\xBF")) {
            bytes = bytes.mid(3);
        }

        return QString::fromUtf8(bytes);
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao carregar arquivo de texto '%1': %2").arg(filename, e.what()));
        return QString();
    }
}

QString loadTextFile(const QString& filename, const QString& folder, const QString& encoding) {
    return load_text_file(filename, folder, encoding);
}
