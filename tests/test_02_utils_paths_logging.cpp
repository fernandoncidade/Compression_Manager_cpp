#include "test_common.hpp"

#include "source/utils/ApplicationPathUtils.hpp"
#include "source/utils/CaminhoPersistenteUtils.hpp"
#include "source/utils/IconUtils.hpp"
#include "source/utils/LogManager.hpp"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QPixmap>
#include <QTemporaryDir>
#include <QTextStream>
#include <thread>
#include <vector>

class TestUtilsPathsLogging : public QObject {
    Q_OBJECT

private slots:
    void testPersistentPathIsolation() {
        const QString envLocal = QDir::cleanPath(qEnvironmentVariable("LOCALAPPDATA"));
        QVERIFY(!envLocal.isEmpty());

        const QString p1 = obter_caminho_persistente();
        const QString p2 = obterCaminhoPersistente();
        QCOMPARE(p1, p2);
        QVERIFY(QDir(p1).exists());
        QVERIFY2(QDir::cleanPath(p1).startsWith(envLocal, Qt::CaseInsensitive),
                 qPrintable("Persistent path escaped sandbox: " + p1));
        QCOMPARE(QFileInfo(p1).fileName(), QStringLiteral("CompressionManager"));

        // Valida escrita e leitura no diretorio persistente
        const QString probeFile = QDir(p1).filePath("sandbox_probe.txt");
        QVERIFY(cm_test::writeBytes(probeFile, QByteArray("ok")));
        QFile f(probeFile);
        QVERIFY(f.open(QIODevice::ReadOnly));
        QCOMPARE(f.readAll(), QByteArray("ok"));
        f.close();
        QFile::remove(probeFile);
    }

    void testApplicationPathUtilsAndBomStripping() {
        const QString b1 = get_app_base_path();
        const QString b2 = getAppBasePath();
        QCOMPARE(b1, b2);
        QVERIFY(QDir(b1).exists());

        const QString aboutPath = get_text_file_path("ABOUT_pt_BR.txt", "ABOUT");
        const QString aboutAlias = getTextFilePath("ABOUT_pt_BR.txt", "ABOUT");
        QCOMPARE(aboutPath, aboutAlias);
        QVERIFY2(QFileInfo::exists(aboutPath), qPrintable("ABOUT_pt_BR.txt not resolved: " + aboutPath));

        const QString aboutContent = load_text_file("ABOUT_pt_BR.txt", "ABOUT");
        QVERIFY(!aboutContent.isEmpty());
        QCOMPARE(aboutContent, loadTextFile("ABOUT_pt_BR.txt", "ABOUT"));

        // Arquivo com UTF-8 BOM precisa retornar texto sem BOM U+FEFF
        const QString appDir = QCoreApplication::applicationDirPath();
        const QString bomTempName = "cm_bom_probe_temp.txt";
        const QString bomFullPath = QDir(appDir).filePath("assets/" + bomTempName);
        QByteArray rawWithBom = QByteArray("\xEF\xBB\xBF") + QByteArray("Conteudo UTF-8 sem BOM residual");
        QVERIFY(cm_test::writeBytes(bomFullPath, rawWithBom));
        const QString loaded = load_text_file(bomTempName);
        QFile::remove(bomFullPath);

        QCOMPARE(loaded, QStringLiteral("Conteudo UTF-8 sem BOM residual"));
        QVERIFY(!loaded.startsWith(QChar(0xFEFF)));

        // Arquivo inexistente nao deve travar e retorna string vazia
        QVERIFY(load_text_file("arquivo_inexistente_123456.txt", "ABOUT").isEmpty());
    }

    void testIconUtilsAllLeanIcons() {
        const QStringList expectedIcons = {
            "Manager-BackUp.ico",
            "pasta.png",
            "arquivo.png",
            "interrogacao.png",
            "clear_button2.png",
            "clear_button3.png",
            "saida.png",
            "winzip4.png",
            "sevenzip3.png",
            "tar1.png",
            "wim.png",
            "extracao4.png",
            "winrar3.png"
        };

        for (const QString& name : expectedIcons) {
            const QString p1 = get_icon_path(name);
            const QString p2 = IconUtils::getIconPath(name);
            QCOMPARE(p1, p2);
            QVERIFY2(QFileInfo::exists(p1), qPrintable("Missing runtime icon: " + name + " -> " + p1));

            QPixmap pm(p1);
            QVERIFY2(!pm.isNull() && pm.width() > 0 && pm.height() > 0,
                     qPrintable("Failed to load icon into QPixmap: " + p1));
        }
    }

    void testLogManagerLevelsQtHandlerAndThreadSafety() {
        LogManager::configure_logging();
        QVERIFY(&LogManager::instance() == LogManager::get_logger());

        const QString logPath = LogManager::get_log_file();
        QCOMPARE(logPath, LogManager::getLogFile());
        QVERIFY2(!logPath.isEmpty() && QFileInfo::exists(logPath),
                 qPrintable("Log file not created: " + logPath));

        const QString envLocal = QDir::cleanPath(qEnvironmentVariable("LOCALAPPDATA"));
        QVERIFY2(QDir::cleanPath(logPath).startsWith(envLocal, Qt::CaseInsensitive),
                 qPrintable("Log file escaped sandbox: " + logPath));

        LogManager::debug("PROBE_DEBUG_TOKEN");
        LogManager::info("PROBE_INFO_TOKEN");
        LogManager::warning("PROBE_WARNING_TOKEN");
        LogManager::error("PROBE_ERROR_TOKEN", true);
        LogManager::critical("PROBE_CRITICAL_TOKEN", true);
        LogManager::get_logger()->log("PROBE_CUSTOM_LEVEL", "NOTICE");

        qWarning("PROBE_QT_WARNING_HANDLER");

        // Estresse concorrente: 8 threads x 40 mensagens unicas = 320 linhas atomicas
        constexpr int kThreads = 8;
        constexpr int kMsgsPerThread = 40;
        std::vector<std::thread> workers;
        workers.reserve(kThreads);
        for (int t = 0; t < kThreads; ++t) {
            workers.emplace_back([t]() {
                for (int m = 0; m < kMsgsPerThread; ++m) {
                    LogManager::info(QString("MT_TOKEN_T%1_M%2").arg(t).arg(m));
                }
            });
        }
        for (auto& w : workers) {
            w.join();
        }

        QFile logFile(logPath);
        QVERIFY(logFile.open(QIODevice::ReadOnly | QIODevice::Text));
        const QString content = QString::fromUtf8(logFile.readAll());
        logFile.close();

        QVERIFY(content.contains("[DEBUG] FileCompression: PROBE_DEBUG_TOKEN"));
        QVERIFY(content.contains("[INFO] FileCompression: PROBE_INFO_TOKEN"));
        QVERIFY(content.contains("[WARNING] FileCompression: PROBE_WARNING_TOKEN"));
        QVERIFY(content.contains("[ERROR] FileCompression: PROBE_ERROR_TOKEN"));
        QVERIFY(content.contains("[CRITICAL] FileCompression: PROBE_CRITICAL_TOKEN"));
        QVERIFY(content.contains("[NOTICE] FileCompression: PROBE_CUSTOM_LEVEL"));
        QVERIFY(content.contains("[WARNING] FileCompression: PROBE_QT_WARNING_HANDLER"));

        for (int t = 0; t < kThreads; ++t) {
            for (int m = 0; m < kMsgsPerThread; ++m) {
                const QString token = QString("MT_TOKEN_T%1_M%2").arg(t).arg(m);
                QVERIFY2(content.contains(token), qPrintable("Missing concurrent log line: " + token));
            }
        }
    }
};

CM_TEST_MAIN(TestUtilsPathsLogging)
#ifndef __INTELLISENSE__
#include "test_02_utils_paths_logging.moc"
#endif
