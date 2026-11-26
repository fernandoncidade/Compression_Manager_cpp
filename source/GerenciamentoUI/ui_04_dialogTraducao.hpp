#pragma once

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QMap>
#include <QPair>

class DialogoTraduzivel : public QDialog {
    Q_OBJECT

public:
    explicit DialogoTraduzivel(QWidget* parent = nullptr);
    ~DialogoTraduzivel() override = default;

    int exec() override;
    void adicionar_texto_traduzir(QWidget* widget, const QString& chave, const QString& contexto = "Dialog");
    void traduzir_interface();

private:
    QMap<QWidget*, QPair<QString, QString>> textos_traduzir;
};

class FileDialogTraduzivel : public QFileDialog {
    Q_OBJECT

public:
    explicit FileDialogTraduzivel(QWidget* parent = nullptr);
    ~FileDialogTraduzivel() override = default;

    int exec() override;
    void traduzir_botoes();

private:
    bool botoes_traduzidos = false;
};

class MessageBoxTraduzivel : public QMessageBox {
    Q_OBJECT

public:
    explicit MessageBoxTraduzivel(QMessageBox::Icon icon,
                                  const QString& title,
                                  const QString& text,
                                  QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                                  QWidget* parent = nullptr);
    ~MessageBoxTraduzivel() override = default;

    int exec() override;
    void traduzir_botoes();
    void atualizar_traducao();

private:
    QString titulo_chave;
    QString mensagem_chave;
};
