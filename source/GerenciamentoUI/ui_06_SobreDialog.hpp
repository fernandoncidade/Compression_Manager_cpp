#pragma once

#include <QDialog>
#include <QString>
#include <QStringList>

class QLabel;
class QTabWidget;
class QPushButton;
class QTextBrowser;

class SobreDialog : public QDialog {
    Q_OBJECT

public:
    SobreDialog(QWidget* parent,
                const QString& titulo,
                const QString& texto_fixo,
                const QString& texto_history,
                const QString& detalhes,
                const QString& licencas,
                const QString& sites_licencas,
                const QString& show_history_text,
                const QString& hide_history_text,
                const QString& show_details_text,
                const QString& hide_details_text,
                const QString& show_licenses_text,
                const QString& hide_licenses_text,
                const QString& ok_text,
                const QString& site_oficial_text,
                const QString& avisos = QString(),
                const QString& show_notices_text = QString(),
                const QString& hide_notices_text = QString(),
                const QString& Privacy_Policy = QString(),
                const QString& show_privacy_policy_text = QString(),
                const QString& hide_privacy_policy_text = QString(),
                const QString& info_not_available_text = "Information not available",
                const QString& release_notes = QString(),
                const QString& show_release_notes_text = QString(),
                const QString& hide_release_notes_text = QString());

    ~SobreDialog() override = default;

    void atualizar_conteudo(
        const QString& titulo,
        const QString& texto_fixo,
        const QString& texto_history,
        const QString& detalhes,
        const QString& licencas,
        const QString& sites_licencas,
        const QString& show_history_text,
        const QString& hide_history_text,
        const QString& show_details_text,
        const QString& hide_details_text,
        const QString& show_licenses_text,
        const QString& hide_licenses_text,
        const QString& ok_text,
        const QString& site_oficial_text,
        const QString& avisos = QString(),
        const QString& show_notices_text = QString(),
        const QString& hide_notices_text = QString(),
        const QString& Privacy_Policy = QString(),
        const QString& show_privacy_policy_text = QString(),
        const QString& hide_privacy_policy_text = QString(),
        const QString& info_not_available_text = "Information not available",
        const QString& release_notes = QString(),
        const QString& show_release_notes_text = QString(),
        const QString& hide_release_notes_text = QString());

protected:
    void changeEvent(QEvent* event) override;

private slots:
    void onTabChanged(int index);

private:
    void updateTabLabels(int currentIndex);

    QLabel* fixed_label = nullptr;
    QTabWidget* tabs = nullptr;
    QPushButton* ok_button = nullptr;

    QTextBrowser* historyBrowser = nullptr;
    QTextBrowser* detalhesBrowser = nullptr;
    QTextBrowser* licencasBrowser = nullptr;
    QTextBrowser* avisosBrowser = nullptr;
    QTextBrowser* privacidadeBrowser = nullptr;
    QTextBrowser* releaseBrowser = nullptr;

    QStringList show_labels;
    QStringList hide_labels;
};
