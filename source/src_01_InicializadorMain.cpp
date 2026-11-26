#include "src_01_InicializadorMain.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/language/tr_01_gerenciadorTraducao.hpp"
#include "source/utils/LogManager.hpp"
#include "source/GerenciamentoUI/ui_03_dragDrop.hpp"
#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFont>
#include <QStyle>
#include <QStyleFactory>
#include <QTextStream>
#include <exception>

#if defined(Q_OS_WIN)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

int iniciar_aplicacao(int argc, char* argv[]) {
    try {
        LogManager::configure_logging();
        LogManager::info("Iniciando aplicação Compression Manager...");
        DragDropListWidget::registrar_diagnostico_privilegios();

        // 1. Identificar o diretório local do executável e garantir caminhos de plugins locais
        QString appDir;
#if defined(Q_OS_WIN)
        wchar_t modPath[MAX_PATH];
        if (GetModuleFileNameW(nullptr, modPath, MAX_PATH) > 0) {
            appDir = QFileInfo(QString::fromWCharArray(modPath)).absolutePath();
        }
#endif
        if (appDir.isEmpty()) {
            if (argc > 0 && argv[0]) {
                appDir = QFileInfo(QString::fromLocal8Bit(argv[0])).absolutePath();
            }
        }

        if (!appDir.isEmpty()) {
            QCoreApplication::addLibraryPath(appDir);
            QCoreApplication::addLibraryPath(appDir + QStringLiteral("/plugins"));
            QCoreApplication::addLibraryPath(appDir + QStringLiteral("/styles"));

            // Garantir existência de qt.conf local para que a inicialização de estilos e plugins seja autônoma
            const QString qtConfPath = appDir + QStringLiteral("/qt.conf");
            if (!QFile::exists(qtConfPath)) {
                QFile qtConfFile(qtConfPath);
                if (qtConfFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream out(&qtConfFile);
                    out << "[Paths]\nPrefix = .\nPlugins = plugins\n";
                    qtConfFile.close();
                }
            }
        }

        QApplication::setDesktopSettingsAware(true);

        QApplication app(argc, argv);
        app.setApplicationName(QStringLiteral("Compression Manager"));
        app.setOrganizationName(QStringLiteral("Fernando Nillsson Cidade"));

        // Reafirmar caminhos de plugins resolvidos em tempo de execução
        const QString runtimeAppDir = QCoreApplication::applicationDirPath();
        if (!runtimeAppDir.isEmpty()) {
            QCoreApplication::addLibraryPath(runtimeAppDir);
            QCoreApplication::addLibraryPath(runtimeAppDir + QStringLiteral("/plugins"));
            QCoreApplication::addLibraryPath(runtimeAppDir + QStringLiteral("/styles"));
        }

        for (const QString &pluginDir : {
            QStringLiteral("C:\\Program Files\\Compression Manager\\plugins"),
            QStringLiteral("C:\\Program Files (x86)\\Compression Manager\\plugins")
        }) {
            if (QDir(pluginDir).exists()) {
                QCoreApplication::addLibraryPath(pluginDir);
            }
        }

        // 2. Aplicar estilo visual sofisticado nativo do Qt6 (Windows 11 Fluent / Vista)
        // Evita o fallback para o visual rústico antigo (Windows 95 Classic) quando executado manualmente
        const QStringList availableStyles = QStyleFactory::keys();
        LogManager::info(QStringLiteral("Estilos visuais Qt6 disponíveis: %1").arg(availableStyles.join(QStringLiteral(", "))));

        bool styleApplied = false;
        const QStringList preferredStyles = {
            QStringLiteral("windows11"),
            QStringLiteral("windowsvista"),
            QStringLiteral("Fusion")
        };

        for (const QString &preferred : preferredStyles) {
            for (const QString &available : availableStyles) {
                if (available.compare(preferred, Qt::CaseInsensitive) == 0) {
                    QStyle *style = QStyleFactory::create(available);
                    if (style) {
                        app.setStyle(style);
                        LogManager::info(QStringLiteral("Estilo visual nativo Qt6 aplicado com sucesso: %1").arg(available));
                        styleApplied = true;
                        break;
                    }
                }
            }
            if (styleApplied) {
                break;
            }
        }

        // 3. Tipografia nativa e refinada (Segoe UI no Windows)
#if defined(Q_OS_WIN)
        QFont modernFont = app.font();
        modernFont.setFamily(QStringLiteral("Segoe UI"));
        modernFont.setPointSize(9);
        modernFont.setStyleHint(QFont::SansSerif);
        app.setFont(modernFont);
#endif


        InterfaceGrafica window;
        window.show();
        DragDropListWidget::habilitar_drop_nativo_se_elevado(&window);

        int exit_code = app.exec();
        return exit_code;
    } catch (const std::exception& e) {
        LogManager::critical(QString("Erro fatal ao iniciar aplicação: %1").arg(e.what()));
        return 1;
    } catch (...) {
        LogManager::critical("Erro fatal desconhecido ao iniciar aplicação");
        return 1;
    }
}
