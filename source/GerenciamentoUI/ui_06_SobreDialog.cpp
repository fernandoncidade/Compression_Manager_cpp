#include "ui_06_SobreDialog.hpp"
#include "source/utils/LogManager.hpp"

#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSizePolicy>
#include <QTabWidget>
#include <QTextBrowser>
#include <QVBoxLayout>

SobreDialog::SobreDialog(QWidget* parent,
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
                         const QString& avisos,
                         const QString& show_notices_text,
                         const QString& hide_notices_text,
                         const QString& Privacy_Policy,
                         const QString& show_privacy_policy_text,
                         const QString& hide_privacy_policy_text,
                         const QString& info_not_available_text,
                         const QString& release_notes,
                         const QString& show_release_notes_text,
                         const QString& hide_release_notes_text)
    : QDialog(parent)
{
    try {
        setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowSystemMenuHint |
                       Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
        setModal(false);

        auto layout = new QVBoxLayout(this);

        auto headerWidget = new QWidget(this);
        auto headerLayout = new QVBoxLayout(headerWidget);
        headerLayout->setContentsMargins(0, 0, 0, 0);
        headerLayout->setSpacing(0);

        fixed_label = new QLabel(headerWidget);
        fixed_label->setTextFormat(Qt::RichText);
        fixed_label->setWordWrap(true);
        fixed_label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        fixed_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        fixed_label->setStyleSheet("QLabel { margin: 0px; padding: 0px; }");
        headerLayout->addWidget(fixed_label);

        headerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        layout->addWidget(headerWidget);

        tabs = new QTabWidget(this);
        tabs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // 1. History
        historyBrowser = new QTextBrowser(tabs);
        historyBrowser->setReadOnly(true);
        historyBrowser->setOpenExternalLinks(true);
        tabs->addTab(historyBrowser, QString());

        // 2. Detalhes
        detalhesBrowser = new QTextBrowser(tabs);
        detalhesBrowser->setReadOnly(true);
        detalhesBrowser->setOpenExternalLinks(true);
        tabs->addTab(detalhesBrowser, QString());

        // 3. Licenças
        licencasBrowser = new QTextBrowser(tabs);
        licencasBrowser->setReadOnly(true);
        licencasBrowser->setOpenExternalLinks(true);
        tabs->addTab(licencasBrowser, QString());

        // 4. Avisos
        avisosBrowser = new QTextBrowser(tabs);
        avisosBrowser->setReadOnly(true);
        avisosBrowser->setOpenExternalLinks(true);
        tabs->addTab(avisosBrowser, QString());

        // 5. Privacidade
        privacidadeBrowser = new QTextBrowser(tabs);
        privacidadeBrowser->setReadOnly(true);
        privacidadeBrowser->setOpenExternalLinks(true);
        tabs->addTab(privacidadeBrowser, QString());

        // 6. Release Notes
        releaseBrowser = new QTextBrowser(tabs);
        releaseBrowser->setReadOnly(true);
        releaseBrowser->setOpenExternalLinks(true);
        tabs->addTab(releaseBrowser, QString());

        connect(tabs, &QTabWidget::currentChanged, this, &SobreDialog::onTabChanged);

        layout->addWidget(tabs);

        auto buttonLayout = new QHBoxLayout();
        ok_button = new QPushButton(this);
        ok_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        connect(ok_button, &QPushButton::clicked, this, &SobreDialog::accept);

        buttonLayout->addStretch(1);
        buttonLayout->addWidget(ok_button);
        layout->addLayout(buttonLayout);

        atualizar_conteudo(
            titulo,
            texto_fixo,
            texto_history,
            detalhes,
            licencas,
            sites_licencas,
            show_history_text,
            hide_history_text,
            show_details_text,
            hide_details_text,
            show_licenses_text,
            hide_licenses_text,
            ok_text,
            site_oficial_text,
            avisos,
            show_notices_text,
            hide_notices_text,
            Privacy_Policy,
            show_privacy_policy_text,
            hide_privacy_policy_text,
            info_not_available_text,
            release_notes,
            show_release_notes_text,
            hide_release_notes_text
        );

        setMinimumSize(400, 200);
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao criar SobreDialog: %1").arg(e.what()));
    }
}

void SobreDialog::atualizar_conteudo(
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
    const QString& avisos,
    const QString& show_notices_text,
    const QString& hide_notices_text,
    const QString& Privacy_Policy,
    const QString& show_privacy_policy_text,
    const QString& hide_privacy_policy_text,
    const QString& info_not_available_text,
    const QString& release_notes,
    const QString& show_release_notes_text,
    const QString& hide_release_notes_text)
{
    try {
        setWindowTitle(titulo);

        if (fixed_label) {
            fixed_label->setText(QString("<div style=\"margin:0;padding:0;\">%1</div>").arg(texto_fixo));
        }

        if (historyBrowser) {
            if (!texto_history.isEmpty()) {
                historyBrowser->setPlainText(texto_history);
            } else {
                historyBrowser->setHtml(QString("<p>%1.</p>").arg(info_not_available_text));
            }
        }

        if (detalhesBrowser) {
            if (!detalhes.isEmpty()) {
                detalhesBrowser->setPlainText(detalhes);
            } else {
                detalhesBrowser->setHtml(QString("<p>%1.</p>").arg(info_not_available_text));
            }
        }

        if (licencasBrowser) {
            if (!licencas.isEmpty()) {
                QString html = licencas;
                html.replace("\n", "<br>");
                html += QString("<br><br><h3>%1</h3><ul>").arg(site_oficial_text);
                const QStringList sites = sites_licencas.split('\n', Qt::SkipEmptyParts);
                for (const QString& site : sites) {
                    QString s = site.trimmed();
                    if (!s.isEmpty()) {
                        html += QString("<li><a href=\"%1\">%1</a></li>").arg(s);
                    }
                }
                html += "</ul>";
                licencasBrowser->setHtml(html);
            } else {
                licencasBrowser->setHtml(QString("<p>%1.</p>").arg(info_not_available_text));
            }
        }

        if (avisosBrowser) {
            if (!avisos.isEmpty()) {
                avisosBrowser->setPlainText(avisos);
            } else {
                avisosBrowser->setHtml(QString("<p>%1.</p>").arg(info_not_available_text));
            }
        }

        if (privacidadeBrowser) {
            if (!Privacy_Policy.isEmpty()) {
                privacidadeBrowser->setPlainText(Privacy_Policy);
            } else {
                privacidadeBrowser->setHtml(QString("<p>%1.</p>").arg(info_not_available_text));
            }
        }

        if (releaseBrowser) {
            if (!release_notes.isEmpty()) {
                releaseBrowser->setPlainText(release_notes);
            } else {
                releaseBrowser->setHtml(QString("<p>%1.</p>").arg(info_not_available_text));
            }
        }

        QString relLabel = show_release_notes_text.isEmpty() ? "Release Notes" : show_release_notes_text;

        show_labels = {
            show_history_text,
            show_details_text,
            show_licenses_text,
            show_notices_text,
            show_privacy_policy_text,
            relLabel
        };

        hide_labels = {
            hide_history_text,
            hide_details_text,
            hide_licenses_text,
            hide_notices_text,
            hide_privacy_policy_text,
            hide_release_notes_text
        };

        if (tabs) {
            updateTabLabels(tabs->currentIndex());
        }

        if (ok_button) {
            ok_button->setText(ok_text);
        }
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao atualizar SobreDialog: %1").arg(e.what()));
    }
}

void SobreDialog::onTabChanged(int index) {
    try {
        updateTabLabels(index);
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao atualizar rótulos de abas: %1").arg(e.what()));
    }
}

void SobreDialog::updateTabLabels(int currentIndex) {
    if (!tabs) return;
    for (int i = 0; i < tabs->count(); ++i) {
        if (i == currentIndex) {
            QString hideLabel = (i < hide_labels.size()) ? hide_labels[i] : QString();
            QString label = !hideLabel.isEmpty() ? hideLabel : ((i < show_labels.size()) ? show_labels[i] : "");
            tabs->setTabText(i, label);
        } else {
            QString label = (i < show_labels.size() && !show_labels[i].isEmpty()) ? show_labels[i] : tabs->tabText(i);
            tabs->setTabText(i, label);
        }
    }
}

void SobreDialog::changeEvent(QEvent* event) {
    if (event->type() == QEvent::ActivationChange) {
        if (isActiveWindow()) {
            raise();
        }
    }
    QDialog::changeEvent(event);
}
