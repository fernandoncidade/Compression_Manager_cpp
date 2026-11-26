#include "mtcomp_02_motoresCompressao.hpp"
#include "source/GerenciamentoUI/ui_03_dragDrop.hpp"
#include "source/utils/ApplicationPathUtils.hpp"
#include "source/utils/LogManager.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QTemporaryDir>

static const QStringList WINRAR_PATHS = {
    "C:\\Program Files\\WinRAR\\WinRAR.exe",
    "C:\\Program Files (x86)\\WinRAR\\WinRAR.exe",
    "C:\\Program Files\\Compression Manager\\plugins\\WinRAR\\WinRAR.exe",
    "C:\\Program Files (x86)\\Compression Manager\\plugins\\WinRAR\\WinRAR.exe",
};

static const QStringList SEVENZIP_PATHS = {
    "C:\\Program Files\\7-Zip\\7zG.exe",
    "C:\\Program Files (x86)\\7-Zip\\7zG.exe",
    "C:\\Program Files\\7-Zip\\7z.exe",
    "C:\\Program Files (x86)\\7-Zip\\7z.exe",
    "C:\\Program Files\\Compression Manager\\plugins\\7Z\\7zG.exe",
    "C:\\Program Files\\Compression Manager\\plugins\\7Z\\7z.exe",
    "C:\\Program Files (x86)\\Compression Manager\\plugins\\7Z\\7zG.exe",
    "C:\\Program Files (x86)\\Compression Manager\\plugins\\7Z\\7z.exe",
};

static const QStringList INSTALLED_PLUGIN_DIRS_WINRAR = {
    "C:\\Program Files\\Compression Manager\\plugins\\WinRAR",
    "C:\\Program Files (x86)\\Compression Manager\\plugins\\WinRAR",
};

static const QStringList INSTALLED_PLUGIN_DIRS_7Z = {
    "C:\\Program Files\\Compression Manager\\plugins\\7Z",
    "C:\\Program Files (x86)\\Compression Manager\\plugins\\7Z",
    "C:\\Program Files\\Compression Manager\\plugins\\7-Zip",
    "C:\\Program Files (x86)\\Compression Manager\\plugins\\7-Zip",
};

QString buscar_diretorio_plugins_7z() {
    try {
        QString app_dir = QCoreApplication::applicationDirPath();
        QString app_base = get_app_base_path();

        QStringList candidatos = {
            QDir(app_dir).filePath("plugins/7Z"),
            QDir(app_dir).filePath("plugins/7-Zip"),
            QDir(app_base).filePath("plugins/7Z"),
            QDir(app_base).filePath("plugins/7-Zip"),
            "C:\\Program Files\\Compression Manager\\plugins\\7Z",
            "C:\\Program Files (x86)\\Compression Manager\\plugins\\7Z",
            "C:\\Program Files\\Compression Manager\\plugins\\7-Zip",
            "C:\\Program Files (x86)\\Compression Manager\\plugins\\7-Zip"
        };

        for (const QString& cand : candidatos) {
            if (QDir(cand).exists()) {
                return QDir(cand).absolutePath();
            }
        }

        return QString();
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao buscar diretório de plugins 7Z: %1").arg(e.what()));
        return QString();
    }
}

QString buscar_diretorio_plugins_winrar() {
    try {
        QString app_dir = QCoreApplication::applicationDirPath();
        QString app_base = get_app_base_path();

        QStringList candidatos = {
            QDir(app_dir).filePath("plugins/WinRAR"),
            QDir(app_base).filePath("plugins/WinRAR"),
            "C:\\Program Files\\Compression Manager\\plugins\\WinRAR",
            "C:\\Program Files (x86)\\Compression Manager\\plugins\\WinRAR"
        };

        for (const QString& cand : candidatos) {
            if (QDir(cand).exists()) {
                return QDir(cand).absolutePath();
            }
        }

        return QString();
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao buscar diretório de plugins WinRAR: %1").arg(e.what()));
        return QString();
    }
}

QString buscar_arquivo_plugin(const QString& nome_pasta, const QString& nome_arquivo) {
    try {
        QString app_dir = QCoreApplication::applicationDirPath();
        QString app_base = get_app_base_path();

        QStringList pastas_teste = { nome_pasta };
        if (nome_pasta.compare("7Z", Qt::CaseInsensitive) == 0) {
            pastas_teste << "7-Zip";
        } else if (nome_pasta.compare("7-Zip", Qt::CaseInsensitive) == 0) {
            pastas_teste << "7Z";
        }

        for (const QString& pasta : pastas_teste) {
            QString sub = QString("plugins/%1/%2").arg(pasta, nome_arquivo);

            QString local_path = QDir(app_dir).filePath(sub);
            if (QFileInfo::exists(local_path)) {
                return QFileInfo(local_path).absoluteFilePath();
            }

            QString base_path = QDir(app_base).filePath(sub);
            if (QFileInfo::exists(base_path)) {
                return QFileInfo(base_path).absoluteFilePath();
            }

            QString prog_path = QString("C:\\Program Files\\Compression Manager\\plugins\\%1\\%2").arg(pasta, nome_arquivo);
            if (QFileInfo::exists(prog_path)) {
                return QFileInfo(prog_path).absoluteFilePath();
            }

            QString prog86_path = QString("C:\\Program Files (x86)\\Compression Manager\\plugins\\%1\\%2").arg(pasta, nome_arquivo);
            if (QFileInfo::exists(prog86_path)) {
                return QFileInfo(prog86_path).absoluteFilePath();
            }
        }

        return QString();
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao buscar arquivo de plugin '%1/%2': %3").arg(nome_pasta, nome_arquivo, e.what()));
        return QString();
    }
}

QString buscar_executavel(const QString& nome_pasta, const QString& nome_executavel, const QStringList& possiveis_caminhos) {
    try {
        // 1. Prioridade: verificar se existe nos caminhos externos do sistema
        for (const QString& caminho : possiveis_caminhos) {
            if (QFileInfo::exists(caminho)) {
                LogManager::info(QString("Executável '%1' encontrado no caminho externo do sistema: %2").arg(nome_executavel, caminho));
                return QFileInfo(caminho).absoluteFilePath();
            }
        }

        // 2. Quando não encontrar no sistema, buscar nos diretórios de plugins (ex: plugins/WinRAR, plugins/7Z)
        QString app_dir = QCoreApplication::applicationDirPath();
        QString app_base_path = get_app_base_path();

        QString sub = QString("plugins/%1").arg(nome_pasta);

        QString local_path = QDir(app_dir).filePath(QString("%1/%2").arg(sub, nome_executavel));
        if (QFileInfo::exists(local_path)) {
            LogManager::info(QString("Executável '%1' encontrado no diretório de plugins copiado: %2").arg(nome_executavel, local_path));
            return QFileInfo(local_path).absoluteFilePath();
        }

        QString bundle_path = QDir(app_base_path).filePath(QString("%1/%2").arg(sub, nome_executavel));
        if (QFileInfo::exists(bundle_path)) {
            LogManager::info(QString("Executável '%1' encontrado no diretório base de plugins: %2").arg(nome_executavel, bundle_path));
            return QFileInfo(bundle_path).absoluteFilePath();
        }

        // 3. Buscar nos diretórios de instalação do Compression Manager (Inno Setup)
        QStringList instalados = {
            QString("C:\\Program Files\\Compression Manager\\plugins\\%1\\%2").arg(nome_pasta, nome_executavel),
            QString("C:\\Program Files (x86)\\Compression Manager\\plugins\\%1\\%2").arg(nome_pasta, nome_executavel)
        };

        for (const QString& inst : instalados) {
            if (QFileInfo::exists(inst)) {
                LogManager::info(QString("Executável '%1' encontrado no diretório de instalação do Compression Manager: %2").arg(nome_executavel, inst));
                return QFileInfo(inst).absoluteFilePath();
            }
        }

        return QString();
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao buscar executável '%1': %2").arg(nome_executavel, e.what()));
        return QString();
    }
}

QString buscar_winrar_executavel() {
#if !ENABLE_RAR_SUPPORT
    return QString();
#else
    try {
        // 1. Verificar caminhos externos padrão no sistema e na pasta de instalação do Compression Manager
        for (const QString& caminho : WINRAR_PATHS) {
            if (QFileInfo::exists(caminho)) {
                LogManager::info(QString("WinRAR encontrado no caminho do sistema / instalação: %1").arg(caminho));
                return QFileInfo(caminho).absoluteFilePath();
            }
        }

        // 2. Fallback: quando não instalado no sistema, buscar nos diretórios de plugins locais e instalados
        QString app_dir = QCoreApplication::applicationDirPath();
        QString app_base = get_app_base_path();

        QStringList pastas_candidatas = {
            QDir(app_dir).filePath("plugins/WinRAR/WinRAR.exe"),
            QDir(app_base).filePath("plugins/WinRAR/WinRAR.exe"),
            "C:\\Program Files\\Compression Manager\\plugins\\WinRAR\\WinRAR.exe",
            "C:\\Program Files (x86)\\Compression Manager\\plugins\\WinRAR\\WinRAR.exe"
        };

        for (const QString& candidato : pastas_candidatas) {
            if (QFileInfo::exists(candidato)) {
                LogManager::info(QString("WinRAR encontrado no diretório de plugins: %1").arg(candidato));
                return QFileInfo(candidato).absoluteFilePath();
            }
        }

        return QString();
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao buscar executável WinRAR: %1").arg(e.what()));
        return QString();
    }
#endif
}

QString buscar_sevenzip_executavel() {
    try {
        // 1. Verificar caminhos externos padrão no sistema e na pasta de instalação do Compression Manager
        for (const QString& caminho : SEVENZIP_PATHS) {
            if (QFileInfo::exists(caminho)) {
                LogManager::info(QString("7-Zip encontrado no caminho do sistema / instalação: %1").arg(caminho));
                return QFileInfo(caminho).absoluteFilePath();
            }
        }

        // 2. Fallback: quando não instalado no sistema, buscar nos diretórios de plugins locais e instalados
        QString app_dir = QCoreApplication::applicationDirPath();
        QString app_base = get_app_base_path();

        QStringList subpastas = {
            "plugins/7Z",
            "plugins/7-Zip"
        };

        QStringList executaveis = {
            "7zG.exe",
            "7z.exe"
        };

        for (const QString& sub : subpastas) {
            for (const QString& exe : executaveis) {
                QString local_path = QDir(app_dir).filePath(QString("%1/%2").arg(sub, exe));
                if (QFileInfo::exists(local_path)) {
                    LogManager::info(QString("7-Zip encontrado no diretório de plugins: %1").arg(local_path));
                    return QFileInfo(local_path).absoluteFilePath();
                }

                QString base_path = QDir(app_base).filePath(QString("%1/%2").arg(sub, exe));
                if (QFileInfo::exists(base_path)) {
                    LogManager::info(QString("7-Zip encontrado no diretório base de plugins: %1").arg(base_path));
                    return QFileInfo(base_path).absoluteFilePath();
                }
            }
        }

        for (const QString& dir_instalado : INSTALLED_PLUGIN_DIRS_7Z) {
            for (const QString& exe : executaveis) {
                QString inst_path = QDir(dir_instalado).filePath(exe);
                if (QFileInfo::exists(inst_path)) {
                    LogManager::info(QString("7-Zip encontrado no diretório de plugins da instalação: %1").arg(inst_path));
                    return QFileInfo(inst_path).absoluteFilePath();
                }
            }
        }

        return QString();
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao buscar executável 7-Zip: %1").arg(e.what()));
        return QString();
    }
}

CompressaoBase::CompressaoBase(const QString& executable,
                               bool update_existing,
                               DragDropListWidget* output_listbox,
                               DragDropListWidget* folder_listbox,
                               bool compress_as,
                               const QString& compression_method,
                               QObject* parent)
    : QThread(parent),
      executable(executable),
      update_existing(update_existing),
      output_listbox(output_listbox),
      folder_listbox(folder_listbox),
      compress_as(compress_as),
      compression_method(compression_method)
{
}

void CompressaoBase::run() {
    try {
        if (!output_listbox || !folder_listbox) {
            emit finished();
            return;
        }

        QStringList output_paths;
        for (int i = 0; i < output_listbox->count(); ++i) {
            output_paths << output_listbox->item(i)->text();
        }

        if (output_paths.isEmpty()) {
            emit finished();
            return;
        }

        for (int idx = 0; idx < folder_listbox->count(); ++idx) {
            QString folder_path = folder_listbox->item(idx)->text();
            QFileInfo fi(folder_path);
            QString folder_name = fi.fileName();
            if (folder_name.isEmpty()) {
                folder_name = fi.dir().dirName();
            }

            for (const QString& output_path : output_paths) {
                comprimir(output_path, folder_path, folder_name);
            }
        }
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro no método run de CompressaoBase: %1").arg(e.what()));
    }
    emit finished();
}

CompressaoRAR::CompressaoRAR(const QString& executable,
                             bool update_existing,
                             DragDropListWidget* output_listbox,
                             DragDropListWidget* folder_listbox,
                             bool compress_as,
                             const QString& compression_method,
                             QObject* parent)
    : CompressaoBase(executable, update_existing, output_listbox, folder_listbox, compress_as, compression_method, parent)
{
    format = "rar";
}

void CompressaoRAR::comprimir(const QString& output_path, const QString& folder_path, const QString& folder_name) {
    try {
        QString compressed_file = QDir(output_path).filePath(QString("%1.rar").arg(folder_name));
        QString op = update_existing ? "u" : "a";
        QString solid_flag = (compression_method != "0" && !compression_method.isEmpty()) ? " -s" : "";
        QString cmd = QString("\"%1\" %2 -r%3 -ep1 -m%4 -afrar \"%5\" \"%6\"")
                          .arg(executable, op, solid_flag, compression_method, compressed_file, folder_path);

        QProcess process;
        process.startCommand(cmd);
        process.waitForFinished(-1);
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao comprimir (RAR) pasta '%1' em '%2': %3").arg(folder_path, output_path, e.what()));
    }
}

CompressaoZIP::CompressaoZIP(const QString& executable,
                             bool update_existing,
                             DragDropListWidget* output_listbox,
                             DragDropListWidget* folder_listbox,
                             bool compress_as,
                             const QString& compression_method,
                             QObject* parent)
    : CompressaoBase(executable, update_existing, output_listbox, folder_listbox, compress_as, compression_method, parent)
{
    format = "zip";
}

void CompressaoZIP::comprimir(const QString& output_path, const QString& folder_path, const QString& folder_name) {
    try {
        QString compressed_file = QDir(output_path).filePath(QString("%1.zip").arg(folder_name));
        QString op = update_existing ? "u" : "a";
        QString cmd = QString("\"%1\" %2 -r -tzip -mx=%3 \"%4\" \"%5\"")
                          .arg(executable, op, compression_method, compressed_file, folder_path);

        QProcess process;
        process.startCommand(cmd);
        process.waitForFinished(-1);
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao comprimir (ZIP) pasta '%1' em '%2': %3").arg(folder_path, output_path, e.what()));
    }
}

Compressao7Z::Compressao7Z(const QString& executable,
                           bool update_existing,
                           DragDropListWidget* output_listbox,
                           DragDropListWidget* folder_listbox,
                           bool compress_as,
                           const QString& compression_method,
                           QObject* parent)
    : CompressaoBase(executable, update_existing, output_listbox, folder_listbox, compress_as, compression_method, parent)
{
    format = "7z";
}

void Compressao7Z::comprimir(const QString& output_path, const QString& folder_path, const QString& folder_name) {
    try {
        QString compressed_file = QDir(output_path).filePath(QString("%1.7z").arg(folder_name));
        QString op = update_existing ? "u" : "a";
        QString cmd = QString("\"%1\" %2 -r -t7z -mx=%3 \"%4\" \"%5\"")
                          .arg(executable, op, compression_method, compressed_file, folder_path);

        QProcess process;
        process.startCommand(cmd);
        process.waitForFinished(-1);
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao comprimir (7Z) pasta '%1' em '%2': %3").arg(folder_path, output_path, e.what()));
    }
}

CompressaoTAR::CompressaoTAR(const QString& executable,
                             bool update_existing,
                             DragDropListWidget* output_listbox,
                             DragDropListWidget* folder_listbox,
                             bool compress_as,
                             const QString& compression_method,
                             QObject* parent)
    : CompressaoBase(executable, update_existing, output_listbox, folder_listbox, compress_as, compression_method, parent)
{
    format = "tar";
}

void CompressaoTAR::comprimir(const QString& output_path, const QString& folder_path, const QString& folder_name) {
    try {
        QString compressed_file = QDir(output_path).filePath(QString("%1.tar").arg(folder_name));
        QString op = update_existing ? "u" : "a";
        QString cmd = QString("\"%1\" %2 -r -ttar \"%3\" \"%4\"")
                          .arg(executable, op, compressed_file, folder_path);

        QProcess process;
        process.startCommand(cmd);
        process.waitForFinished(-1);
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao comprimir (TAR) pasta '%1' em '%2': %3").arg(folder_path, output_path, e.what()));
    }
}

CompressaoWIM::CompressaoWIM(const QString& executable,
                             bool update_existing,
                             DragDropListWidget* output_listbox,
                             DragDropListWidget* folder_listbox,
                             bool compress_as,
                             const QString& compression_method,
                             QObject* parent)
    : CompressaoBase(executable, update_existing, output_listbox, folder_listbox, compress_as, compression_method, parent)
{
    format = "wim";
}

void CompressaoWIM::comprimir(const QString& output_path, const QString& folder_path, const QString& folder_name) {
    try {
        QString compressed_file = QDir(output_path).filePath(QString("%1.wim").arg(folder_name));
        QString op = update_existing ? "u" : "a";
        QString cmd = QString("\"%1\" %2 -r -twim \"%3\" \"%4\"")
                          .arg(executable, op, compressed_file, folder_path);

        QProcess process;
        process.startCommand(cmd);
        process.waitForFinished(-1);
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao comprimir (WIM) pasta '%1' em '%2': %3").arg(folder_path, output_path, e.what()));
    }
}

TesteIntegridade::TesteIntegridade(const QString& sevenzip_executable, const QStringList& compressed_files, QObject* parent)
    : QThread(parent),
      sevenzip_executable(sevenzip_executable),
      compressed_files(compressed_files)
{
}

void TesteIntegridade::run() {
    try {
        const QStringList supportedExts = {
            ".rar", ".zip", ".7z", ".bz2", ".tar", ".wim", ".tar.xz", ".zipx", ".tgz", ".tar.gz", ".lzh", ".iso"
        };

        for (const QString& compressed_file : compressed_files) {
            QString lower = compressed_file.toLower();
            bool matches = false;
            for (const QString& ext : supportedExts) {
                if (lower.endsWith(ext)) {
                    matches = true;
                    break;
                }
            }

            if (matches) {
                QString cmd = QString("\"%1\" t \"%2\"").arg(sevenzip_executable, compressed_file);
                QProcess process;
                process.startCommand(cmd);
                process.waitForFinished(-1);
                if (process.exitCode() != 0) {
                    LogManager::error("Falha na verificação de integridade do arquivo: " + compressed_file);
                }
            }
        }
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro no método run de TesteIntegridade: %1").arg(e.what()));
    }
    emit finished();
}

Extracao::Extracao(const QString& sevenzip_executable,
                   DragDropListWidget* output_listbox,
                   DragDropListWidget* folder_listbox,
                   QObject* parent)
    : QThread(parent),
      sevenzip_executable(sevenzip_executable),
      output_listbox(output_listbox),
      folder_listbox(folder_listbox)
{
}

void Extracao::run() {
    try {
        if (!output_listbox || !folder_listbox) {
            emit finished();
            return;
        }

        QStringList output_paths;
        for (int idx = 0; idx < output_listbox->count(); ++idx) {
            output_paths << output_listbox->item(idx)->text();
        }

        if (output_paths.isEmpty()) {
            emit finished();
            return;
        }

        for (int idx = 0; idx < folder_listbox->count(); ++idx) {
            QString archive_path = folder_listbox->item(idx)->text();
            for (const QString& output_path : output_paths) {
                extrair(archive_path, output_path);
            }
        }
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro no método run de Extracao: %1").arg(e.what()));
    }
    emit finished();
}

void Extracao::extrair(const QString& archive_path, const QString& output_path) {
    try {
        QString lower = archive_path.toLower();

        if (lower.endsWith(".rar") || lower.endsWith(".zip") || lower.endsWith(".7z") ||
            lower.endsWith(".tar") || lower.endsWith(".wim")) {
            QString cmd = QString("\"%1\" x -y \"%2\" -o\"%3\"").arg(sevenzip_executable, archive_path, output_path);
            QProcess process;
            process.startCommand(cmd);
            process.waitForFinished(-1);
        } else if (lower.endsWith(".bz2")) {
            QTemporaryDir tempDir;
            if (tempDir.isValid()) {
                QFileInfo fi(archive_path);
                QString base = fi.fileName();
                base.remove(".bz2", Qt::CaseInsensitive);
                QString tempTar = QDir(tempDir.path()).filePath(base);

                QString cmd1 = QString("\"%1\" x -y \"%2\" -o\"%3\"").arg(sevenzip_executable, archive_path, tempDir.path());
                QProcess process1;
                process1.startCommand(cmd1);
                process1.waitForFinished(-1);

                QString cmd2 = QString("\"%1\" x -y \"%2\" -o\"%3\"").arg(sevenzip_executable, tempTar, output_path);
                QProcess process2;
                process2.startCommand(cmd2);
                process2.waitForFinished(-1);
            }
        } else if (lower.endsWith(".tar.xz") || lower.endsWith(".tgz") || lower.endsWith(".tar.gz")) {
            QTemporaryDir tempDir;
            if (tempDir.isValid()) {
                QFileInfo fi(archive_path);
                QString base = fi.fileName();
                base.replace(".xz", "", Qt::CaseInsensitive);
                base.replace(".tgz", ".tar", Qt::CaseInsensitive);
                base.replace(".gz", "", Qt::CaseInsensitive);
                QString tempTar = QDir(tempDir.path()).filePath(base);

                QString cmd1 = QString("\"%1\" x -y \"%2\" -o\"%3\"").arg(sevenzip_executable, archive_path, tempDir.path());
                QProcess process1;
                process1.startCommand(cmd1);
                process1.waitForFinished(-1);

                QString cmd2 = QString("\"%1\" x -y \"%2\" -o\"%3\"").arg(sevenzip_executable, tempTar, output_path);
                QProcess process2;
                process2.startCommand(cmd2);
                process2.waitForFinished(-1);
            }
        } else if (lower.endsWith(".zipx") || lower.endsWith(".lzh") || lower.endsWith(".iso")) {
            QString cmd = QString("\"%1\" x -y \"%2\" -o\"%3\"").arg(sevenzip_executable, archive_path, output_path);
            QProcess process;
            process.startCommand(cmd);
            process.waitForFinished(-1);
        }
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro no método extrair de Extracao: %1").arg(e.what()));
    }
}
