#pragma once

#include <QListWidget>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QShowEvent>
#include <QStringList>

class DragDropListWidget : public QListWidget {
    Q_OBJECT

public:
    explicit DragDropListWidget(QWidget* parent = nullptr, bool accept_folders_only = false);
    ~DragDropListWidget() override = default;

    bool accept_folders_only;

    void adicionar_caminhos(const QStringList& paths);

    // Retorna true quando o processo atual está rodando elevado (Administrador / High Integrity).
    static bool processo_esta_elevado();

    // Grava no log o nível de integridade do processo (diagnóstico de UIPI).
    static void registrar_diagnostico_privilegios();

    // Em processo elevado, o OLE Drag & Drop do Qt é bloqueado pelo UIPI do Windows.
    // Esta função revoga o alvo OLE da janela e habilita o caminho legado WM_DROPFILES,
    // que o Windows permite atravessar níveis de integridade quando liberado via
    // ChangeWindowMessageFilterEx. Não faz nada quando o processo não está elevado.
    static void habilitar_drop_nativo_se_elevado(QWidget* janela);

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void showEvent(QShowEvent* event) override;
    bool viewportEvent(QEvent* event) override;

private:
    bool isDirectory(const QString& path);
    void configurar_drag_drop();
};
