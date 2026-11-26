#pragma once

#include <QAbstractButton>
#include <QApplication>
#include <QByteArray>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDir>
#include <QDirIterator>
#include <QElapsedTimer>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QMap>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QSet>
#include <QString>
#include <QTimer>
#include <QtTest/QtTest>

#ifndef CM_PROJECT_SOURCE_DIR
#define CM_PROJECT_SOURCE_DIR ""
#endif
#ifndef CM_TEST_BINARY_DIR
#define CM_TEST_BINARY_DIR ""
#endif
#ifndef CM_TEST_TOOLCHAIN
#define CM_TEST_TOOLCHAIN "unknown"
#endif
#ifndef CM_QT_VERSION_EXPECTED
#define CM_QT_VERSION_EXPECTED "6.12.0"
#endif

namespace cm_test {

inline QString sourceRootDir() {
    return QDir::cleanPath(QString::fromUtf8(CM_PROJECT_SOURCE_DIR));
}

inline QString binaryRootDir() {
    return QDir::cleanPath(QString::fromUtf8(CM_TEST_BINARY_DIR));
}

inline QString toolchainName() {
    return QString::fromUtf8(CM_TEST_TOOLCHAIN);
}

inline QString prepareIsolatedSandbox(const QString& suiteName) {
    const QString sandboxPath = QDir(binaryRootDir()).filePath(QStringLiteral(".test_sandbox/") + suiteName);
    QDir dir(sandboxPath);
    if (dir.exists()) {
        dir.removeRecursively();
    }
    QDir().mkpath(sandboxPath);

    qputenv("LOCALAPPDATA", QDir::toNativeSeparators(sandboxPath).toUtf8());
    if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM")) {
        qputenv("QT_QPA_PLATFORM", "offscreen");
    }
    return QDir::cleanPath(sandboxPath);
}

inline void configureQtLibraryPaths() {
    const QString appDir = QCoreApplication::applicationDirPath();
    if (!appDir.isEmpty()) {
        QCoreApplication::addLibraryPath(appDir);
        QCoreApplication::addLibraryPath(appDir + QStringLiteral("/plugins"));
        QCoreApplication::addLibraryPath(appDir + QStringLiteral("/plugins/platforms"));
        QCoreApplication::addLibraryPath(appDir + QStringLiteral("/plugins/styles"));
        QCoreApplication::addLibraryPath(appDir + QStringLiteral("/plugins/imageformats"));
    }
}

inline QByteArray sha256File(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QByteArray();
    }
    QCryptographicHash hash(QCryptographicHash::Sha256);
    if (!hash.addData(&file)) {
        return QByteArray();
    }
    return hash.result().toHex();
}

inline QMap<QString, QByteArray> collectDirectoryHashes(const QString& rootDir) {
    QMap<QString, QByteArray> result;
    const QDir base(rootDir);
    if (!base.exists()) {
        return result;
    }

    QDirIterator it(rootDir, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        const QString absPath = it.next();
        const QString relPath = QDir::fromNativeSeparators(base.relativeFilePath(absPath));
        result.insert(relPath, sha256File(absPath));
    }
    return result;
}

inline bool writeBytes(const QString& filePath, const QByteArray& content) {
    const QFileInfo fi(filePath);
    if (!QDir().mkpath(fi.absolutePath())) {
        return false;
    }
    QFile f(filePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }
    const qint64 written = f.write(content);
    f.close();
    return written == content.size();
}

inline bool createRichFixtureTree(const QString& targetDir) {
    if (!QDir().mkpath(targetDir)) {
        return false;
    }
    const QDir root(targetDir);

    // 1. Documento UTF-8 com acentuacao e quebras de linha
    if (!writeBytes(root.filePath(QStringLiteral("relatório_técnico_ação.txt")),
                    QByteArray("Compression Manager C++23 / Qt 6.12.0 Verification Payload\nLine 2: Integração completa\n"))) {
        return false;
    }

    // 2. Arquivo com espacos no nome
    if (!writeBytes(root.filePath("arquivo com espacos.txt"),
                    QByteArray(4096, 'Z'))) {
        return false;
    }

    // 3. Arquivo binario deterministico de 128 KiB em subdiretorio aninhado
    QByteArray binaryPayload;
    binaryPayload.resize(131072);
    quint32 state = 0x12345678u;
    for (int i = 0; i < binaryPayload.size(); ++i) {
        state = state * 1664525u + 1013904223u;
        binaryPayload[i] = static_cast<char>((state >> 24) & 0xFF);
    }
    if (!writeBytes(root.filePath("sub_a/sub_b/dados_binarios.bin"), binaryPayload)) {
        return false;
    }

    // 4. Arquivo vazio (0 bytes)
    if (!writeBytes(root.filePath("sub_a/vazio.dat"), QByteArray())) {
        return false;
    }

    return true;
}

inline QString findConsoleSevenZip() {
    const QString appDir = QCoreApplication::applicationDirPath();
    const QStringList candidates = {
        QDir(appDir).filePath("plugins/7Z/7z.exe"),
        QDir(binaryRootDir()).filePath("plugins/7Z/7z.exe"),
        QStringLiteral("C:/Program Files/7-Zip/7z.exe"),
        QStringLiteral("C:/Program Files (x86)/7-Zip/7z.exe")
    };
    for (const QString& c : candidates) {
        if (QFileInfo::exists(c) && QFileInfo(c).isFile()) {
            return QFileInfo(c).absoluteFilePath();
        }
    }
    return QString();
}

struct CapturedModalDialog {
    QMessageBox::Icon icon = QMessageBox::NoIcon;
    QString title;
    QString text;
    QStringList buttonTexts;
};

class ModalMessageBoxInterceptor : public QObject {
public:
    explicit ModalMessageBoxInterceptor(QMessageBox::StandardButton buttonToClick = QMessageBox::Ok,
                                        QObject* parent = nullptr)
        : QObject(parent),
          m_buttonToClick(buttonToClick)
    {
        connect(&m_timer, &QTimer::timeout, this, &ModalMessageBoxInterceptor::pollAndDismiss);
        m_timer.start(20);
    }

    ~ModalMessageBoxInterceptor() override {
        m_timer.stop();
    }

    void setButtonToClick(QMessageBox::StandardButton button) {
        m_buttonToClick = button;
    }

    const QList<CapturedModalDialog>& captured() const {
        return m_captured;
    }

    int count() const {
        return static_cast<int>(m_captured.size());
    }

    void clear() {
        m_captured.clear();
        m_seen.clear();
    }

    bool waitForCount(int expectedCount, int timeoutMs = 15000) {
        QElapsedTimer timer;
        timer.start();
        while (count() < expectedCount && timer.elapsed() < timeoutMs) {
            pollAndDismiss();
            QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
            QTest::qWait(20);
        }
        pollAndDismiss();
        return count() >= expectedCount;
    }

private:
    void pollAndDismiss() {
        auto inspectWidget = [this](QWidget* w) {
            auto* mb = qobject_cast<QMessageBox*>(w);
            if (!mb || !mb->isVisible() || m_seen.contains(mb)) {
                return;
            }
            m_seen.insert(mb);
            connect(mb, &QObject::destroyed, this, [this, mb]() {
                m_seen.remove(mb);
            });

            CapturedModalDialog info;
            info.icon = mb->icon();
            info.title = mb->windowTitle();
            info.text = mb->text();
            for (QAbstractButton* b : mb->buttons()) {
                if (b) {
                    info.buttonTexts.append(b->text());
                }
            }
            m_captured.append(info);

            if (QAbstractButton* targetBtn = mb->button(m_buttonToClick)) {
                targetBtn->click();
            } else if (QAbstractButton* defBtn = mb->defaultButton()) {
                defBtn->click();
            } else {
                mb->done(m_buttonToClick);
            }
        };

        if (QWidget* active = QApplication::activeModalWidget()) {
            inspectWidget(active);
        }
        const auto tops = QApplication::topLevelWidgets();
        for (QWidget* top : tops) {
            inspectWidget(top);
        }
    }

    QTimer m_timer;
    QMessageBox::StandardButton m_buttonToClick = QMessageBox::Ok;
    QSet<QMessageBox*> m_seen;
    QList<CapturedModalDialog> m_captured;
};

} // namespace cm_test

#define CM_TEST_MAIN(TestClass)                                              \
    int main(int argc, char* argv[]) {                                       \
        cm_test::prepareIsolatedSandbox(QStringLiteral(#TestClass));         \
        QApplication app(argc, argv);                                        \
        cm_test::configureQtLibraryPaths();                                  \
        TestClass tc;                                                        \
        return QTest::qExec(&tc, argc, argv);                                \
    }
