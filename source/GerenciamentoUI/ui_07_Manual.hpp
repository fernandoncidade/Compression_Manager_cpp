// MANUAL DO USUÁRIO — Compression Manager
// Versão: 2026.10.17.0 (17 de outubro de 2026)

#pragma once

#include <QDialog>
#include <QString>

class QTextBrowser;
class QPushButton;

class ManualDialog : public QDialog {
    Q_OBJECT

public:
    explicit ManualDialog(QWidget* parent = nullptr, const QString& idioma = "pt_BR");
    ~ManualDialog() override = default;

    void atualizar_traducao(const QString& idioma);

protected:
    void changeEvent(QEvent* event) override;

private:
    QTextBrowser* browser = nullptr;
    QPushButton* ok_button = nullptr;
};

void exibir_manual(QWidget* app);
