#include "tr_02_compileTranslations.hpp"
#include "source/utils/ApplicationPathUtils.hpp"
#include "source/utils/LogManager.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLibraryInfo>
#include <QProcess>
#include <iostream>

namespace TranslationCompiler {

QString encontrar_lrelease() {
    QStringList candidates = {
        QLibraryInfo::path(QLibraryInfo::BinariesPath) + "/lrelease.exe",
        QLibraryInfo::path(QLibraryInfo::BinariesPath) + "/lrelease",
        "C:/Qt/6.11.1/mingw_64/bin/lrelease.exe",
        "C:/Qt/6.11.1/msvc2022_64/bin/lrelease.exe",
        "lrelease.exe",
        "lrelease"
    };

    for (const QString& candidate : candidates) {
        if (QFile::exists(candidate)) {
            return candidate;
        }
    }
    return "lrelease";
}

bool compilar_traducoes() {
    QString baseDir = get_app_base_path();
    QString transDir = QDir(baseDir).filePath("language/translations");
    if (!QDir(transDir).exists()) {
        transDir = QDir(baseDir).filePath("source/language/translations");
    }

    QDir dir(transDir);
    if (!dir.exists()) {
        LogManager::error("Diretório de traduções não encontrado: " + transDir);
        return false;
    }

    QString lrelease = encontrar_lrelease();
    QStringList tsFiles = dir.entryList(QStringList() << "*.ts", QDir::Files);
    bool allSuccess = true;

    for (const QString& tsName : tsFiles) {
        QString tsPath = dir.filePath(tsName);
        QString qmName = tsName;
        qmName.replace(".ts", ".qm");
        QString qmPath = dir.filePath(qmName);

        LogManager::info("Compilando tradução: " + tsName);

        QStringList args;
        args << tsPath << "-qm" << qmPath;

        QProcess process;
        process.start(lrelease, args);
        if (!process.waitForFinished(10000)) {
            LogManager::error("Timeout compilando " + tsName);
            allSuccess = false;
            continue;
        }

        if (process.exitCode() == 0) {
            LogManager::info("Sucesso compilando " + tsName + " -> " + qmName);
        } else {
            LogManager::error("Erro compilando " + tsName + ": " + process.readAllStandardError());
            allSuccess = false;
        }
    }

    return allSuccess;
}

} // namespace TranslationCompiler
