#include "test_common.hpp"

#include "source/GerenciamentoUI/ui_03_dragDrop.hpp"
#include "source/utils/LogManager.hpp"

#include <QDir>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QFile>
#include <QFileInfo>
#include <QMimeData>
#include <QPointF>
#include <QTemporaryDir>
#include <QUrl>

class TestDragDropUipiWidgets : public QObject {
    Q_OBJECT

private slots:
    void testInputModeFilesFoldersAndCaseInsensitiveDedup() {
        QTemporaryDir temp;
        QVERIFY(temp.isValid());
        const QDir root(temp.path());
        const QString folderA = root.filePath("FolderAlpha");
        const QString fileA = root.filePath("FolderAlpha/sample_input.txt");
        const QString fileB = root.filePath("second_input.dat");
        QVERIFY(QDir().mkpath(folderA));
        QVERIFY(cm_test::writeBytes(fileA, QByteArray("abc")));
        QVERIFY(cm_test::writeBytes(fileB, QByteArray("123")));

        DragDropListWidget list(nullptr, /*accept_folders_only=*/false);
        QVERIFY(!list.accept_folders_only);
        QVERIFY(list.acceptDrops());
        QVERIFY(list.viewport()->acceptDrops());

        // Adiciona pasta, arquivo e duplicata com separadores e caixa distintos
        list.adicionar_caminhos({
            folderA,
            fileA,
            QString(), // vazio ignorado
            QDir::fromNativeSeparators(fileA).toUpper(), // duplicata case-insensitive
            QDir::toNativeSeparators(folderA) + QDir::separator() + ".." + QDir::separator() + "FolderAlpha"
        });

        QCOMPARE(list.count(), 2);

        // Evento sintetico dragEnter + dragMove + drop via QMimeData URLs
        QMimeData mimeUrls;
        mimeUrls.setUrls({ QUrl::fromLocalFile(fileB) });

        QDragEnterEvent enterEv(QPointF(5, 5), Qt::CopyAction, &mimeUrls, Qt::LeftButton, Qt::NoModifier);
        QCoreApplication::sendEvent(list.viewport(), &enterEv);
        QVERIFY(enterEv.isAccepted());

        QDragMoveEvent moveEv(QPointF(5, 5), Qt::CopyAction, &mimeUrls, Qt::LeftButton, Qt::NoModifier);
        QCoreApplication::sendEvent(list.viewport(), &moveEv);
        QVERIFY(moveEv.isAccepted());

        QDropEvent dropEv(QPointF(5, 5), Qt::CopyAction, &mimeUrls, Qt::LeftButton, Qt::NoModifier);
        QCoreApplication::sendEvent(list.viewport(), &dropEv);
        QVERIFY(dropEv.isAccepted());
        QCOMPARE(list.count(), 3);
        QCOMPARE(QDir::cleanPath(list.item(2)->text()), QDir::cleanPath(fileB));
    }

    void testOutputModeExtractsParentDirAndRejectsInvalidMime() {
        QTemporaryDir temp;
        QVERIFY(temp.isValid());
        const QDir root(temp.path());
        const QString outDir1 = root.filePath("OutDirOne");
        const QString outDir2 = root.filePath("OutDirTwo");
        const QString nestedFile1 = outDir1 + "/inner1.bin";
        const QString nestedFile2 = outDir1 + "/INNER2.bin";
        QVERIFY(QDir().mkpath(outDir1));
        QVERIFY(QDir().mkpath(outDir2));
        QVERIFY(cm_test::writeBytes(nestedFile1, QByteArray("x")));
        QVERIFY(cm_test::writeBytes(nestedFile2, QByteArray("y")));

        DragDropListWidget list(nullptr, /*accept_folders_only=*/true);
        QVERIFY(list.accept_folders_only);

        // Quando recebe arquivo no modo saida, precisa extrair e normalizar a pasta pai e deduplicar
        list.adicionar_caminhos({ nestedFile1, nestedFile2 });
        QCOMPARE(list.count(), 1);
        QCOMPARE(QDir::cleanPath(list.item(0)->text()), QDir::cleanPath(outDir1));

        // Drop via text/uri-list de outra pasta (fluxo completo DragEnter -> DragMove -> Drop)
        QMimeData uriMime;
        uriMime.setData("text/uri-list", QUrl::fromLocalFile(outDir2).toEncoded() + "\r\n");
        QDragEnterEvent uriEnter(QPointF(5, 5), Qt::CopyAction, &uriMime, Qt::LeftButton, Qt::NoModifier);
        QCoreApplication::sendEvent(list.viewport(), &uriEnter);
        QDragMoveEvent uriMove(QPointF(5, 5), Qt::CopyAction, &uriMime, Qt::LeftButton, Qt::NoModifier);
        QCoreApplication::sendEvent(list.viewport(), &uriMove);
        QDropEvent uriDrop(QPointF(5, 5), Qt::CopyAction, &uriMime, Qt::LeftButton, Qt::NoModifier);
        QCoreApplication::sendEvent(list.viewport(), &uriDrop);
        QVERIFY(uriEnter.isAccepted());
        QVERIFY(uriMove.isAccepted());
        QCOMPARE(list.count(), 2);
        QCOMPARE(QDir::cleanPath(list.item(1)->text()), QDir::cleanPath(outDir2));

        // Rejeita MIME invalido (application/octet-stream)
        QMimeData badMime;
        badMime.setData("application/octet-stream", QByteArray("\x01\x02\x03", 3));
        QDragEnterEvent badEnter(QPointF(5, 5), Qt::CopyAction, &badMime, Qt::LeftButton, Qt::NoModifier);
        QCoreApplication::sendEvent(list.viewport(), &badEnter);
        QVERIFY(!badEnter.isAccepted());

        QDropEvent badDrop(QPointF(5, 5), Qt::CopyAction, &badMime, Qt::LeftButton, Qt::NoModifier);
        QCoreApplication::sendEvent(list.viewport(), &badDrop);
        QVERIFY(!badDrop.isAccepted());
        QCOMPARE(list.count(), 2);
    }

    void testUipiPrivilegeDiagnosticsSmoke() {
        // Verifica ausencia de crash e registro consistente no log
        const bool elevated1 = DragDropListWidget::processo_esta_elevado();
        const bool elevated2 = DragDropListWidget::processo_esta_elevado();
        QCOMPARE(elevated1, elevated2);

        DragDropListWidget::registrar_diagnostico_privilegios();
        const QString logPath = LogManager::get_log_file();
        QFile f(logPath);
        QVERIFY(f.open(QIODevice::ReadOnly | QIODevice::Text));
        const QString logText = QString::fromUtf8(f.readAll());
        f.close();
        QVERIFY(logText.contains("Privilégios do processo: elevado="));

        DragDropListWidget widget;
        DragDropListWidget::habilitar_drop_nativo_se_elevado(nullptr);
        DragDropListWidget::habilitar_drop_nativo_se_elevado(&widget);
    }
};

CM_TEST_MAIN(TestDragDropUipiWidgets)
#ifndef __INTELLISENSE__
#include "test_06_dragdrop_uipi_widgets.moc"
#endif
