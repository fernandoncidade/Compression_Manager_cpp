#include "ui_03_dragDrop.hpp"
#include "source/utils/LogManager.hpp"
#include <QDir>
#include <QFileInfo>
#include <QMimeData>
#include <QUrl>
#include <QRegularExpression>
#include <QApplication>
#include <QTimer>
#include <QPointer>

#ifdef Q_OS_WIN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <ole2.h>
#include <shellapi.h>
#include <QAbstractNativeEventFilter>
#include <vector>

#ifndef WM_COPYGLOBALDATA
#define WM_COPYGLOBALDATA 0x0049
#endif
#ifndef MSGFLT_ALLOW
#define MSGFLT_ALLOW 1
#endif

typedef BOOL(WINAPI* PFN_ChangeWindowMessageFilterEx)(HWND, UINT, DWORD, PCHANGEFILTERSTRUCT);

namespace {

// Localiza a DragDropListWidget sob o ponto de soltura informado pelo WM_DROPFILES.
// As coordenadas do Windows são físicas (pixels reais); o Qt trabalha com coordenadas
// lógicas, por isso a conversão pelo devicePixelRatio é obrigatória em telas com escala.
DragDropListWidget* localizar_lista_alvo(HWND hwnd, HDROP hDrop) {
    POINT pt{};
    DragQueryPoint(hDrop, &pt);
    POINT screenPt = pt;
    ClientToScreen(hwnd, &screenPt);

    HWND ownerHwnd = hwnd;
    QWidget* owner = nullptr;
    while (ownerHwnd && !owner) {
        owner = QWidget::find(reinterpret_cast<WId>(ownerHwnd));
        if (!owner) {
            ownerHwnd = GetParent(ownerHwnd);
        }
    }
    if (!owner) {
        return nullptr;
    }

    POINT ownerPt = screenPt;
    ScreenToClient(ownerHwnd, &ownerPt);
    qreal dpr = owner->devicePixelRatioF();
    if (dpr <= 0.0) {
        dpr = 1.0;
    }
    const QPoint local(qRound(ownerPt.x / dpr), qRound(ownerPt.y / dpr));

    QWidget* hit = owner->childAt(local);
    if (!hit) {
        hit = owner;
    }
    for (QWidget* w = hit; w; w = w->parentWidget()) {
        if (auto* lista = qobject_cast<DragDropListWidget*>(w)) {
            return lista;
        }
    }
    return nullptr;
}

class WinNativeDropEventFilter : public QAbstractNativeEventFilter {
public:
    bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) override {
        if (eventType != "windows_generic_MSG" && eventType != "windows_dispatcher_MSG") {
            return false;
        }
        MSG* msg = static_cast<MSG*>(message);
        if (!msg || msg->message != WM_DROPFILES) {
            return false;
        }
        HDROP hDrop = reinterpret_cast<HDROP>(msg->wParam);
        if (!hDrop) {
            return false;
        }

        DragDropListWidget* alvo = localizar_lista_alvo(msg->hwnd, hDrop);

        const UINT fileCount = DragQueryFileW(hDrop, 0xFFFFFFFF, nullptr, 0);
        QStringList paths;
        paths.reserve(static_cast<int>(fileCount));
        for (UINT i = 0; i < fileCount; ++i) {
            const UINT len = DragQueryFileW(hDrop, i, nullptr, 0);
            if (len > 0) {
                std::vector<wchar_t> buffer(len + 1, L'\0');
                DragQueryFileW(hDrop, i, buffer.data(), len + 1);
                paths.append(QString::fromWCharArray(buffer.data()));
            }
        }
        DragFinish(hDrop);

        if (alvo) {
            alvo->adicionar_caminhos(paths);
        } else {
            LogManager::warning("WM_DROPFILES recebido fora das áreas de entrada/saída; itens ignorados.");
        }

        if (result) {
            *result = 0;
        }
        return true;
    }
};

void instalar_filtro_nativo() {
    static WinNativeDropEventFilter* s_filtro = nullptr;
    if (!s_filtro && qApp) {
        s_filtro = new WinNativeDropEventFilter();
        qApp->installNativeEventFilter(s_filtro);
    }
}

void liberar_drop_no_hwnd(HWND hwnd, PFN_ChangeWindowMessageFilterEx pChangeFilterEx) {
    if (!hwnd) {
        return;
    }
    // Enquanto existir um IDropTarget OLE registrado, o Explorer tenta o OLE (bloqueado
    // pelo UIPI) e NUNCA recorre ao WM_DROPFILES. Por isso o alvo OLE precisa ser revogado.
    RevokeDragDrop(hwnd);
    if (pChangeFilterEx) {
        pChangeFilterEx(hwnd, WM_DROPFILES, MSGFLT_ALLOW, nullptr);
        pChangeFilterEx(hwnd, WM_COPYDATA, MSGFLT_ALLOW, nullptr);
        pChangeFilterEx(hwnd, WM_COPYGLOBALDATA, MSGFLT_ALLOW, nullptr);
    }
    DragAcceptFiles(hwnd, TRUE);
}

BOOL CALLBACK enum_child_proc(HWND hwnd, LPARAM lParam) {
    liberar_drop_no_hwnd(hwnd, reinterpret_cast<PFN_ChangeWindowMessageFilterEx>(lParam));
    return TRUE;
}

} // namespace
#endif

bool DragDropListWidget::processo_esta_elevado() {
#ifdef Q_OS_WIN
    static int s_cache = -1;
    if (s_cache >= 0) {
        return s_cache == 1;
    }
    bool elevado = false;
    HANDLE hToken = nullptr;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION elevation{};
        DWORD size = 0;
        if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &size)) {
            elevado = elevation.TokenIsElevated != 0;
        }
        CloseHandle(hToken);
    }
    s_cache = elevado ? 1 : 0;
    return elevado;
#else
    return false;
#endif
}

void DragDropListWidget::registrar_diagnostico_privilegios() {
#ifdef Q_OS_WIN
    QString integridade = QStringLiteral("desconhecida");
    HANDLE hToken = nullptr;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        DWORD size = 0;
        GetTokenInformation(hToken, TokenIntegrityLevel, nullptr, 0, &size);
        if (size > 0) {
            std::vector<BYTE> buffer(size);
            auto* label = reinterpret_cast<TOKEN_MANDATORY_LABEL*>(buffer.data());
            if (GetTokenInformation(hToken, TokenIntegrityLevel, label, size, &size)) {
                const DWORD rid = *GetSidSubAuthority(label->Label.Sid,
                    static_cast<DWORD>(*GetSidSubAuthorityCount(label->Label.Sid) - 1));
                if (rid >= SECURITY_MANDATORY_SYSTEM_RID) {
                    integridade = QStringLiteral("System");
                } else if (rid >= SECURITY_MANDATORY_HIGH_RID) {
                    integridade = QStringLiteral("High (Administrador)");
                } else if (rid >= SECURITY_MANDATORY_MEDIUM_RID) {
                    integridade = QStringLiteral("Medium (usuário padrão)");
                } else {
                    integridade = QStringLiteral("Low");
                }
            }
        }
        CloseHandle(hToken);
    }
    const QString compatLayer = qEnvironmentVariable("__COMPAT_LAYER");
    LogManager::info(QStringLiteral("Privilégios do processo: elevado=%1, integridade=%2, __COMPAT_LAYER=\"%3\"")
                         .arg(processo_esta_elevado() ? QStringLiteral("sim") : QStringLiteral("não"),
                              integridade, compatLayer));
#endif
}

void DragDropListWidget::habilitar_drop_nativo_se_elevado(QWidget* janela) {
#ifdef Q_OS_WIN
    if (!janela || !processo_esta_elevado()) {
        return;
    }
    QWidget* topo = janela->window();
    if (!topo) {
        return;
    }
    instalar_filtro_nativo();

    HMODULE hUser32 = GetModuleHandleW(L"user32.dll");
    auto pChangeFilterEx = hUser32
        ? reinterpret_cast<PFN_ChangeWindowMessageFilterEx>(GetProcAddress(hUser32, "ChangeWindowMessageFilterEx"))
        : nullptr;

    HWND hwndTopo = reinterpret_cast<HWND>(topo->winId());
    liberar_drop_no_hwnd(hwndTopo, pChangeFilterEx);
    EnumChildWindows(hwndTopo, enum_child_proc, reinterpret_cast<LPARAM>(pChangeFilterEx));
#else
    Q_UNUSED(janela);
#endif
}

DragDropListWidget::DragDropListWidget(QWidget* parent, bool accept_folders_only)
    : QListWidget(parent),
      accept_folders_only(accept_folders_only)
{
    configurar_drag_drop();
}

void DragDropListWidget::configurar_drag_drop() {
    setAcceptDrops(true);
    if (viewport()) {
        viewport()->setAcceptDrops(true);
    }
    setDefaultDropAction(Qt::CopyAction);
}

void DragDropListWidget::showEvent(QShowEvent* event) {
    QListWidget::showEvent(event);
    configurar_drag_drop();

    if (processo_esta_elevado()) {
        // Adiado para depois que o Qt concluir a criação/registro da janela nativa;
        // também cobre listas recriadas depois (ex.: troca de idioma).
        QPointer<DragDropListWidget> self(this);
        QTimer::singleShot(0, this, [self]() {
            if (self) {
                DragDropListWidget::habilitar_drop_nativo_se_elevado(self);
            }
        });
    }
}

bool DragDropListWidget::viewportEvent(QEvent* event) {
    if (event->type() == QEvent::DragEnter) {
        dragEnterEvent(static_cast<QDragEnterEvent*>(event));
        return event->isAccepted();
    }
    if (event->type() == QEvent::DragMove) {
        dragMoveEvent(static_cast<QDragMoveEvent*>(event));
        return event->isAccepted();
    }
    if (event->type() == QEvent::DragLeave) {
        dragLeaveEvent(static_cast<QDragLeaveEvent*>(event));
        return event->isAccepted();
    }
    if (event->type() == QEvent::Drop) {
        dropEvent(static_cast<QDropEvent*>(event));
        return event->isAccepted();
    }
    return QListWidget::viewportEvent(event);
}

void DragDropListWidget::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData() && (event->mimeData()->hasUrls() || event->mimeData()->hasFormat("text/uri-list"))) {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void DragDropListWidget::dragMoveEvent(QDragMoveEvent* event) {
    if (event->mimeData() && (event->mimeData()->hasUrls() || event->mimeData()->hasFormat("text/uri-list"))) {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void DragDropListWidget::dragLeaveEvent(QDragLeaveEvent* event) {
    event->accept();
}

void DragDropListWidget::adicionar_caminhos(const QStringList& paths) {
    for (const QString& rawPath : paths) {
        if (rawPath.isEmpty()) {
            continue;
        }

        QString filePath = QDir::toNativeSeparators(QDir::cleanPath(rawPath));
        QFileInfo fi(filePath);

        if (accept_folders_only) {
            // Se o campo for de saída (pastas apenas):
            // Se arrastou um diretório, adiciona o diretório.
            // Se arrastou um arquivo, adiciona o diretório onde o arquivo reside.
            QString dirPath = fi.isDir() ? filePath : fi.absolutePath();
            dirPath = QDir::toNativeSeparators(QDir::cleanPath(dirPath));

            bool exists = false;
            for (int i = 0; i < count(); ++i) {
                if (item(i)->text().compare(dirPath, Qt::CaseInsensitive) == 0) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                addItem(dirPath);
            }
        } else {
            // Campo de entrada: aceita tanto arquivos quanto diretórios!
            bool exists = false;
            for (int i = 0; i < count(); ++i) {
                if (item(i)->text().compare(filePath, Qt::CaseInsensitive) == 0) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                addItem(filePath);
            }
        }
    }
}

void DragDropListWidget::dropEvent(QDropEvent* event) {
    if (event->mimeData() && (event->mimeData()->hasUrls() || event->mimeData()->hasFormat("text/uri-list"))) {
        event->setDropAction(Qt::CopyAction);
        event->accept();

        QList<QUrl> urls = event->mimeData()->urls();
        if (urls.isEmpty() && event->mimeData()->hasText()) {
            const QString text = event->mimeData()->text();
            const QStringList lines = text.split(QRegularExpression("[\r\n]+"), Qt::SkipEmptyParts);
            for (const QString& line : lines) {
                QString trimmed = line.trimmed();
                if (!trimmed.isEmpty()) {
                    urls.append(QUrl::fromUserInput(trimmed));
                }
            }
        }

        QStringList paths;
        paths.reserve(urls.size());
        for (const QUrl& url : urls) {
            QString filePath = url.toLocalFile();
            if (filePath.isEmpty()) {
                filePath = url.path();
#ifdef Q_OS_WIN
                if (filePath.startsWith('/') && filePath.length() > 2 && filePath[2] == ':') {
                    filePath = filePath.mid(1);
                }
#endif
            }
            if (!filePath.isEmpty()) {
                paths.append(filePath);
            }
        }

        adicionar_caminhos(paths);
    } else {
        event->ignore();
    }
}

bool DragDropListWidget::isDirectory(const QString& path) {
    QFileInfo fi(path);
    return fi.isDir();
}
