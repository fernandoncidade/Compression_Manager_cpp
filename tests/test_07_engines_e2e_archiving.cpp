#include "test_common.hpp"

#include "source/GerenciamentoUI/ui_02_gerenteGUILayouts.hpp"
#include "source/GerenciamentoUI/ui_03_dragDrop.hpp"
#include "source/MotoresCompressao/mtcomp_02_motoresCompressao.hpp"
#include "source/utils/LogManager.hpp"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QThread>

namespace {

class SimulatedSlowZipWorker : public CompressaoBase {
public:
    SimulatedSlowZipWorker(DragDropListWidget* outLb, DragDropListWidget* inLb, QObject* parent = nullptr)
        : CompressaoBase(QStringLiteral("simulated"), false, outLb, inLb, true, QStringLiteral("5"), parent)
    {
        format = QStringLiteral("zip");
    }

    void comprimir(const QString& output_path, const QString& /*folder_path*/, const QString& folder_name) override {
        QThread::msleep(500);
        cm_test::writeBytes(QDir(output_path).filePath(folder_name + "_sim.zip"), QByteArray("SIMULATED_ZIP_DATA"));
    }
};

class SimulatedQuick7zWorker : public CompressaoBase {
public:
    SimulatedQuick7zWorker(DragDropListWidget* outLb, DragDropListWidget* inLb, QObject* parent = nullptr)
        : CompressaoBase(QStringLiteral("simulated"), false, outLb, inLb, true, QStringLiteral("5"), parent)
    {
        format = QStringLiteral("7z");
    }

    void comprimir(const QString& output_path, const QString& /*folder_path*/, const QString& folder_name) override {
        QThread::msleep(60);
        cm_test::writeBytes(QDir(output_path).filePath(folder_name + "_sim.7z"), QByteArray("SIMULATED_7Z_DATA"));
    }
};

QMap<QString, QByteArray> resolveExtractedTreeHashes(const QString& extractDir, const QString& expectedSubfolderName) {
    const QString nested = QDir(extractDir).filePath(expectedSubfolderName);
    if (QDir(nested).exists()) {
        return cm_test::collectDirectoryHashes(nested);
    }
    return cm_test::collectDirectoryHashes(extractDir);
}

bool runCmdSync(const QString& exe, const QStringList& args, int timeoutMs = 30000) {
    QProcess p;
    p.start(exe, args);
    if (!p.waitForStarted(5000)) return false;
    if (!p.waitForFinished(timeoutMs)) return false;
    return p.exitStatus() == QProcess::NormalExit && p.exitCode() == 0;
}

} // namespace

class TestEnginesE2eArchiving : public QObject {
    Q_OBJECT

private:
    QString m_sevenZipConsole;
    QString m_winRarExe;

private slots:
    void initTestCase() {
        m_sevenZipConsole = cm_test::findConsoleSevenZip();
        QVERIFY2(!m_sevenZipConsole.isEmpty(), "Console 7z.exe not found for E2E archiving tests");
        m_winRarExe = buscar_winrar_executavel();
    }

    void testPluginDiscoveryAndSecurityAbsenceOfRarRegKey() {
        const QString appDir = QCoreApplication::applicationDirPath();
        const QString dir7z = buscar_diretorio_plugins_7z();
        QVERIFY2(!dir7z.isEmpty() && QDir(dir7z).exists(), qPrintable("7Z plugin dir missing: " + dir7z));

        const QString sevenZipDiscovered = buscar_sevenzip_executavel();
        QVERIFY2(!sevenZipDiscovered.isEmpty() && QFileInfo::exists(sevenZipDiscovered),
                 qPrintable("7-Zip discovery failed: " + sevenZipDiscovered));

        const QString exeDirect = buscar_arquivo_plugin("7Z", "7z.exe");
        const QString dllAlias = buscar_arquivo_plugin("7-Zip", "7z.dll");
        QVERIFY(QFileInfo::exists(exeDirect));
        QVERIFY(QFileInfo::exists(dllAlias));

        // Garantia critica de licenciamento e seguranca: rarreg.key NUNCA pode ser distribuido
        QVERIFY2(!QFileInfo::exists(QDir(appDir).filePath("plugins/WinRAR/rarreg.key")),
                 "SECURITY VIOLATION: rarreg.key present in suite plugins/WinRAR!");
        QVERIFY2(QFileInfo::exists(QDir(appDir).filePath("plugins/WinRAR/WinRAR.exe")),
                 "Expected plugins/WinRAR/WinRAR.exe in Full suite deploy");

        const QString dirRar = buscar_diretorio_plugins_winrar();
        QVERIFY(!dirRar.isEmpty());
        QVERIFY(!m_winRarExe.isEmpty());
    }

    void testZipSevenZipTarWimRoundTripMultiOutUpdateAndSha256() {
        QTemporaryDir temp;
        QVERIFY(temp.isValid());
        const QDir root(temp.path());

        const QString srcFolderName = QStringLiteral("payload_src");
        const QString srcDir = root.filePath(srcFolderName);
        QVERIFY(cm_test::createRichFixtureTree(srcDir));
        const QMap<QString, QByteArray> expectedHashes = cm_test::collectDirectoryHashes(srcDir);
        QCOMPARE(expectedHashes.size(), 4);

        auto verifyArchiveExtraction = [&](const QString& archivePath, const QMap<QString, QByteArray>& expected) {
            QTemporaryDir outTmp;
            QVERIFY(outTmp.isValid());

            DragDropListWidget inLb(nullptr, false);
            DragDropListWidget outLb(nullptr, true);
            inLb.adicionar_caminhos({ archivePath });
            outLb.adicionar_caminhos({ outTmp.path() });

            Extracao extractor(m_sevenZipConsole, &outLb, &inLb);
            QSignalSpy spy(&extractor, &Extracao::finished);
            extractor.start();
            QVERIFY2(extractor.wait(30000), qPrintable("Extracao timed out on: " + archivePath));
            QCoreApplication::processEvents();
            QCOMPARE(spy.count(), 1);

            const QMap<QString, QByteArray> actual = resolveExtractedTreeHashes(outTmp.path(), srcFolderName);
            QCOMPARE(actual, expected);
        };

        // 1. ZIP niveis 0, 5, 9 + Multi-diretorio de saida
        for (const QString& level : { "0", "5", "9" }) {
            const QString outDir1 = root.filePath("out_zip_a_" + level);
            const QString outDir2 = root.filePath("out_zip_b_" + level);
            QVERIFY(QDir().mkpath(outDir1));
            QVERIFY(QDir().mkpath(outDir2));

            DragDropListWidget inLb(nullptr, false);
            DragDropListWidget outLb(nullptr, true);
            inLb.adicionar_caminhos({ srcDir });
            outLb.adicionar_caminhos({ outDir1, outDir2 });

            CompressaoZIP worker(m_sevenZipConsole, false, &outLb, &inLb, true, level);
            QSignalSpy spy(&worker, &CompressaoBase::finished);
            worker.start();
            QVERIFY(worker.wait(30000));
            QCoreApplication::processEvents();
            QCOMPARE(spy.count(), 1);

            const QString z1 = QDir(outDir1).filePath(srcFolderName + ".zip");
            const QString z2 = QDir(outDir2).filePath(srcFolderName + ".zip");
            QVERIFY(QFileInfo::exists(z1) && QFileInfo(z1).size() > 0);
            QVERIFY(QFileInfo::exists(z2) && QFileInfo(z2).size() > 0);
            verifyArchiveExtraction(z1, expectedHashes);
            verifyArchiveExtraction(z2, expectedHashes);
        }

        // 2. 7Z niveis 0 e 5 + Update existing (u)
        {
            const QString out7zDir = root.filePath("out_7z");
            QVERIFY(QDir().mkpath(out7zDir));

            DragDropListWidget inLb(nullptr, false);
            DragDropListWidget outLb(nullptr, true);
            inLb.adicionar_caminhos({ srcDir });
            outLb.adicionar_caminhos({ out7zDir });

            Compressao7Z create7z(m_sevenZipConsole, false, &outLb, &inLb, true, "5");
            create7z.start();
            QVERIFY(create7z.wait(30000));

            const QString archive7z = QDir(out7zDir).filePath(srcFolderName + ".7z");
            QVERIFY(QFileInfo::exists(archive7z));
            verifyArchiveExtraction(archive7z, expectedHashes);

            // Atualiza pacote adicionando novo arquivo na origem com update_existing=true
            QVERIFY(cm_test::writeBytes(QDir(srcDir).filePath("incremental_update.txt"), QByteArray("NEW_UPDATE_CONTENT")));
            const QMap<QString, QByteArray> updatedHashes = cm_test::collectDirectoryHashes(srcDir);
            QCOMPARE(updatedHashes.size(), 5);

            Compressao7Z update7z(m_sevenZipConsole, /*update_existing=*/true, &outLb, &inLb, true, "5");
            update7z.start();
            QVERIFY(update7z.wait(30000));
            verifyArchiveExtraction(archive7z, updatedHashes);
        }

        // 3. TAR (nivel 0) e WIM (nivel 0) sobre a arvore atualizada
        const QMap<QString, QByteArray> latestHashes = cm_test::collectDirectoryHashes(srcDir);
        {
            const QString outTarDir = root.filePath("out_tar");
            const QString outWimDir = root.filePath("out_wim");
            QVERIFY(QDir().mkpath(outTarDir));
            QVERIFY(QDir().mkpath(outWimDir));

            DragDropListWidget inLb(nullptr, false);
            inLb.adicionar_caminhos({ srcDir });

            DragDropListWidget outTarLb(nullptr, true);
            outTarLb.adicionar_caminhos({ outTarDir });
            CompressaoTAR tarWorker(m_sevenZipConsole, false, &outTarLb, &inLb, true, "0");
            tarWorker.start();
            QVERIFY(tarWorker.wait(30000));
            const QString tarPath = QDir(outTarDir).filePath(srcFolderName + ".tar");
            QVERIFY(QFileInfo::exists(tarPath));
            verifyArchiveExtraction(tarPath, latestHashes);

            DragDropListWidget outWimLb(nullptr, true);
            outWimLb.adicionar_caminhos({ outWimDir });
            CompressaoWIM wimWorker(m_sevenZipConsole, false, &outWimLb, &inLb, true, "0");
            wimWorker.start();
            QVERIFY(wimWorker.wait(30000));
            const QString wimPath = QDir(outWimDir).filePath(srcFolderName + ".wim");
            QVERIFY(QFileInfo::exists(wimPath));
            verifyArchiveExtraction(wimPath, latestHashes);
        }

        // 4. RAR via WinRAR.exe (se disponivel em plugins/WinRAR ou sistema)
        if (!m_winRarExe.isEmpty()) {
            const QString outRarDir = root.filePath("out_rar");
            QVERIFY(QDir().mkpath(outRarDir));
            DragDropListWidget inLb(nullptr, false);
            DragDropListWidget outRarLb(nullptr, true);
            inLb.adicionar_caminhos({ srcDir });
            outRarLb.adicionar_caminhos({ outRarDir });

            CompressaoRAR rarWorker(m_winRarExe, false, &outRarLb, &inLb, true, "3");
            rarWorker.start();
            QVERIFY(rarWorker.wait(30000));
            const QString rarPath = QDir(outRarDir).filePath(srcFolderName + ".rar");
            QVERIFY2(QFileInfo::exists(rarPath) && QFileInfo(rarPath).size() > 0,
                     qPrintable("WinRAR failed to produce .rar archive: " + rarPath));
            verifyArchiveExtraction(rarPath, latestHashes);
        }
    }

    void testCompoundTwoStageExtractionPipeline() {
        QTemporaryDir temp;
        QVERIFY(temp.isValid());
        const QDir root(temp.path());

        const QString srcName = "compound_payload";
        const QString srcDir = root.filePath(srcName);
        QVERIFY(cm_test::createRichFixtureTree(srcDir));
        const QMap<QString, QByteArray> expected = cm_test::collectDirectoryHashes(srcDir);

        struct CaseSpec {
            QString tarName;
            QString finalName;
            QString typeFlag;
        };
        const QList<CaseSpec> cases = {
            { "pkg_gz.tar",  "pkg_gz.tar.gz",  "-tgzip" },
            { "pkg_tgz.tar", "pkg_tgz.tgz",    "-tgzip" },
            { "pkg_xz.tar",  "pkg_xz.tar.xz",  "-txz"   },
            { "pkg_bz2.tar", "pkg_bz2.tar.bz2", "-tbzip2" }
        };

        for (const CaseSpec& cs : cases) {
            const QString stageTar = root.filePath(cs.tarName);
            const QString finalArch = root.filePath(cs.finalName);
            const QString destDir = root.filePath("out_" + cs.tarName);
            QVERIFY(QDir().mkpath(destDir));

            QVERIFY(runCmdSync(m_sevenZipConsole, { "a", "-r", "-ttar", stageTar, srcDir }));
            QVERIFY(runCmdSync(m_sevenZipConsole, { "a", cs.typeFlag, finalArch, stageTar }));
            QVERIFY(QFileInfo::exists(finalArch));

            DragDropListWidget inLb(nullptr, false);
            DragDropListWidget outLb(nullptr, true);
            inLb.adicionar_caminhos({ finalArch });
            outLb.adicionar_caminhos({ destDir });

            Extracao extractor(m_sevenZipConsole, &outLb, &inLb);
            extractor.start();
            QVERIFY2(extractor.wait(30000), qPrintable("Compound extraction timed out for: " + cs.finalName));

            const QMap<QString, QByteArray> actual = resolveExtractedTreeHashes(destDir, srcName);
            QCOMPARE(actual, expected);
        }
    }

    void testIntegrityTestingPassFailAndUnsupportedExtensions() {
        QTemporaryDir temp;
        QVERIFY(temp.isValid());
        const QDir root(temp.path());

        const QString srcDir = root.filePath("int_src");
        QVERIFY(cm_test::createRichFixtureTree(srcDir));

        const QString validZip = root.filePath("valid.zip");
        const QString corruptZip = root.filePath("corrupt_archive_probe.zip");
        const QString ignoredTxt = root.filePath("ignored_note.txt");
        QVERIFY(runCmdSync(m_sevenZipConsole, { "a", "-r", "-tzip", validZip, srcDir }));
        QVERIFY(cm_test::writeBytes(corruptZip, QByteArray("PK\x03\x04_CORRUPTED_GARBAGE_BYTES_0123456789")));
        QVERIFY(cm_test::writeBytes(ignoredTxt, QByteArray("not an archive")));

        auto countLogFailures = []() {
            QFile f(LogManager::get_log_file());
            if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return 0;
            const QString text = QString::fromUtf8(f.readAll());
            f.close();
            return static_cast<int>(text.count("Falha na verificação de integridade do arquivo:"));
        };

        // Arquivo valido e .txt ignorado nao geram falha de integridade
        const int beforeValid = countLogFailures();
        {
            TesteIntegridade tiValid(m_sevenZipConsole, { validZip, ignoredTxt });
            QSignalSpy spy(&tiValid, &TesteIntegridade::finished);
            tiValid.start();
            QVERIFY(tiValid.wait(30000));
            QCoreApplication::processEvents();
            QCOMPARE(spy.count(), 1);
        }
        QCOMPARE(countLogFailures(), beforeValid);

        // Arquivo corrompido registra erro no LogManager sem travar
        {
            TesteIntegridade tiCorrupt(m_sevenZipConsole, { corruptZip });
            QSignalSpy spy(&tiCorrupt, &TesteIntegridade::finished);
            tiCorrupt.start();
            QVERIFY(tiCorrupt.wait(30000));
            QCoreApplication::processEvents();
            QCOMPARE(spy.count(), 1);
        }
        QCOMPARE(countLogFailures(), beforeValid + 1);
    }

    void testGerenciadorInterfaceQueueConcurrencyAndModalWarnings() {
        QTemporaryDir temp;
        QVERIFY(temp.isValid());
        const QDir root(temp.path());
        const QString inDir = root.filePath("queue_in");
        const QString outDir = root.filePath("queue_out");
        const QString txtFile = root.filePath("plain.txt");
        QVERIFY(QDir().mkpath(inDir));
        QVERIFY(QDir().mkpath(outDir));
        QVERIFY(cm_test::writeBytes(txtFile, QByteArray("plain")));

        cm_test::ModalMessageBoxInterceptor modalInterceptor(QMessageBox::Ok);
        GerenciadorInterface gi;
        gi.sevenzip_executavel = m_sevenZipConsole;

        // 1. Avisos modais de pre-condicoes
        modalInterceptor.clear();
        gi.store_as_zip(); // sem pasta de entrada
        QCOMPARE(modalInterceptor.count(), 1);

        gi.folder_listbox->adicionar_caminhos({ inDir });
        modalInterceptor.clear();
        gi.store_as_zip(); // sem diretorio de saida
        QCOMPARE(modalInterceptor.count(), 1);

        gi.output_listbox_zip->adicionar_caminhos({ outDir });
        modalInterceptor.clear();
        gi.store_as_zip(); // sem metodo de compressao selecionado
        QCOMPARE(modalInterceptor.count(), 1);

        gi.setCompressionMethod("zip", "5");
        QCOMPARE(gi.getCompressionMethod("zip"), QStringLiteral("5"));

        // Aviso de extensao invalida em testar_integridade + Aviso de extracao sem destino
        gi.clear_folders();
        gi.folder_listbox->adicionar_caminhos({ txtFile });
        modalInterceptor.clear();
        gi.testar_integridade();
        QCOMPARE(modalInterceptor.count(), 1);

        modalInterceptor.clear();
        gi.extract_files(); // sem pasta de saida de extracao
        QCOMPARE(modalInterceptor.count(), 1);

        // Limpezas individuais e totais
        gi.clear_folders();
        QCOMPARE(gi.folder_listbox->count(), 0);
        gi.clear_output();
        QCOMPARE(gi.output_listbox_zip->count(), 0);

        // 2. Enfileiramento deterministico por formato + paralelismo entre formatos distintos
        gi.folder_listbox->adicionar_caminhos({ inDir });
        gi.output_listbox_zip->adicionar_caminhos({ outDir });
        modalInterceptor.clear();

        auto* wZip1 = new SimulatedSlowZipWorker(gi.output_listbox_zip, gi.folder_listbox, &gi);
        auto* wZip2 = new SimulatedSlowZipWorker(gi.output_listbox_zip, gi.folder_listbox, &gi);
        auto* w7z   = new SimulatedQuick7zWorker(gi.output_listbox_zip, gi.folder_listbox, &gi);

        QObject::connect(wZip1, &CompressaoBase::finished, &gi, &GerenciadorInterface::on_compress_finished);
        QObject::connect(wZip2, &CompressaoBase::finished, &gi, &GerenciadorInterface::on_compress_finished);
        QObject::connect(w7z,   &CompressaoBase::finished, &gi, &GerenciadorInterface::on_compress_finished);

        gi.start_compression_thread(wZip1);
        QCOMPARE(gi.compress_threads.size(), 1);
        QCOMPARE(gi.compress_queue.size(), 0);

        // Segundo ZIP com o primeiro ainda em execucao precisa entrar em compress_queue e exibir aviso de fila
        gi.start_compression_thread(wZip2);
        QCOMPARE(gi.compress_queue.size(), 1);
        QCOMPARE(modalInterceptor.count(), 1);
        QCOMPARE(modalInterceptor.captured().first().icon, QMessageBox::Warning);

        // Formato 7Z diferente inicia em paralelo sem aguardar o ZIP
        gi.start_compression_thread(w7z);
        QCOMPARE(gi.compress_threads.size(), 2);

        // Aguarda encerramento completo da fila (w7z finaliza durante wZip1 -> dispara 2o aviso de fila;
        // depois wZip1 e wZip2 concluem e disparam dialogo Information de conclusao)
        QElapsedTimer queueTimer;
        queueTimer.start();
        bool sawInfoCompletion = false;
        while (queueTimer.elapsed() < 15000) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
            for (const auto& dlg : modalInterceptor.captured()) {
                if (dlg.icon == QMessageBox::Information) {
                    sawInfoCompletion = true;
                    break;
                }
            }
            if (sawInfoCompletion && gi.compress_queue.isEmpty() && gi.compress_threads.isEmpty()) {
                break;
            }
            QTest::qWait(25);
        }

        QVERIFY2(sawInfoCompletion, "Completion Information modal not captured after queue drained");
        QCOMPARE(gi.compress_queue.size(), 0);
        QCOMPARE(gi.compress_threads.size(), 0);
        QVERIFY(QFileInfo::exists(QDir(outDir).filePath("queue_in_sim.zip")));
        QVERIFY(QFileInfo::exists(QDir(outDir).filePath("queue_in_sim.7z")));
    }
};

CM_TEST_MAIN(TestEnginesE2eArchiving)
#ifndef __INTELLISENSE__
#include "test_07_engines_e2e_archiving.moc"
#endif
