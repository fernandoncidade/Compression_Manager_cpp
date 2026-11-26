#include "ui_04_dialogTraducao.hpp"
#include <QApplication>
#include <QCoreApplication>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>

static QWidget* resolve_active_parent(QWidget* parent) {
    if (parent && parent->isVisible()) {
        return parent;
    }
    if (qApp) {
        if (QWidget* active = qApp->activeWindow()) {
            if (active->isVisible()) {
                return active;
            }
        }
        const auto topWidgets = QApplication::topLevelWidgets();
        for (QWidget* top : topWidgets) {
            if (top && top->isVisible() && !qobject_cast<QDialog*>(top)) {
                return top;
            }
        }
    }
    return parent;
}

DialogoTraduzivel::DialogoTraduzivel(QWidget* parent)
    : QDialog(resolve_active_parent(parent))
{
    setWindowModality(Qt::ApplicationModal);
    setWindowFlag(Qt::WindowStaysOnTopHint, true);
}

int DialogoTraduzivel::exec() {
    QWidget* target = resolve_active_parent(parentWidget());
    if (target && target != parentWidget()) {
        setParent(target, windowFlags());
    }
    setWindowModality(Qt::ApplicationModal);
    setWindowFlag(Qt::WindowStaysOnTopHint, true);
    raise();
    activateWindow();
    return QDialog::exec();
}

void DialogoTraduzivel::adicionar_texto_traduzir(QWidget* widget, const QString& chave, const QString& contexto) {
    if (widget) {
        textos_traduzir[widget] = qMakePair(chave, contexto);
    }
}

void DialogoTraduzivel::traduzir_interface() {
    for (auto it = textos_traduzir.begin(); it != textos_traduzir.end(); ++it) {
        QWidget* w = it.key();
        QString chave = it.value().first;
        QString ctx = it.value().second;
        QString trad = QCoreApplication::translate(ctx.toUtf8().constData(), chave.toUtf8().constData());

        if (auto lbl = qobject_cast<QLabel*>(w)) {
            lbl->setText(trad);
        } else if (auto btn = qobject_cast<QPushButton*>(w)) {
            btn->setText(trad);
        } else if (auto grp = qobject_cast<QGroupBox*>(w)) {
            grp->setTitle(trad);
        }
    }
}

FileDialogTraduzivel::FileDialogTraduzivel(QWidget* parent)
    : QFileDialog(resolve_active_parent(parent))
{
    setWindowModality(Qt::ApplicationModal);
    setWindowFlag(Qt::WindowStaysOnTopHint, true);
}

int FileDialogTraduzivel::exec() {
    QWidget* target = resolve_active_parent(parentWidget());
    if (target && target != parentWidget()) {
        setParent(target, windowFlags());
    }
    setWindowModality(Qt::ApplicationModal);
    setWindowFlag(Qt::WindowStaysOnTopHint, true);
    raise();
    activateWindow();
    return QFileDialog::exec();
}

void FileDialogTraduzivel::traduzir_botoes() {
    if (!botoes_traduzidos) {
        setLabelText(QFileDialog::LookIn, QCoreApplication::translate("FileDialog", "Procurar em"));
        setLabelText(QFileDialog::FileName, QCoreApplication::translate("FileDialog", "Nome do arquivo"));
        setLabelText(QFileDialog::FileType, QCoreApplication::translate("FileDialog", "Tipo de arquivo"));
        setLabelText(QFileDialog::Accept, QCoreApplication::translate("FileDialog", "Abrir"));
        setLabelText(QFileDialog::Reject, QCoreApplication::translate("FileDialog", "Cancelar"));
        botoes_traduzidos = true;
    }
}

MessageBoxTraduzivel::MessageBoxTraduzivel(QMessageBox::Icon icon,
                                           const QString& title,
                                           const QString& text,
                                           QMessageBox::StandardButtons buttons,
                                           QWidget* parent)
    : QMessageBox(icon, title, text, buttons, resolve_active_parent(parent)),
      titulo_chave(title),
      mensagem_chave(text)
{
    setWindowModality(Qt::ApplicationModal);
    setWindowFlag(Qt::WindowStaysOnTopHint, true);
    traduzir_botoes();
}

int MessageBoxTraduzivel::exec() {
    QWidget* target = resolve_active_parent(parentWidget());
    if (target && target != parentWidget()) {
        setParent(target, windowFlags());
    }
    setWindowModality(Qt::ApplicationModal);
    setWindowFlag(Qt::WindowStaysOnTopHint, true);
    raise();
    activateWindow();
    return QMessageBox::exec();
}

void MessageBoxTraduzivel::traduzir_botoes() {
    struct BtnMap {
        QMessageBox::StandardButton btn;
        const char* texto;
    };
    const BtnMap map[] = {
        { QMessageBox::Ok, "OK" },
        { QMessageBox::Cancel, "Cancelar" },
        { QMessageBox::Yes, "Sim" },
        { QMessageBox::No, "Não" },
        { QMessageBox::Abort, "Abortar" },
        { QMessageBox::Retry, "Tentar Novamente" },
        { QMessageBox::Ignore, "Ignorar" },
        { QMessageBox::Close, "Fechar" },
        { QMessageBox::Help, "Ajuda" },
        { QMessageBox::Apply, "Aplicar" },
        { QMessageBox::Reset, "Redefinir" },
        { QMessageBox::RestoreDefaults, "Restaurar Padrões" },
        { QMessageBox::Save, "Salvar" },
        { QMessageBox::SaveAll, "Salvar Tudo" },
        { QMessageBox::Open, "Abrir" },
    };

    for (const auto& item : map) {
        QAbstractButton* b = button(item.btn);
        if (b) {
            b->setText(QCoreApplication::translate("Dialog", item.texto));
        }
    }
}

void MessageBoxTraduzivel::atualizar_traducao() {
    setWindowTitle(QCoreApplication::translate("InterfaceGrafica", titulo_chave.toUtf8().constData()));
    setText(QCoreApplication::translate("InterfaceGrafica", mensagem_chave.toUtf8().constData()));
    traduzir_botoes();
}
