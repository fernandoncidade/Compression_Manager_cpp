#include "ui_01_layoutsCompressao.hpp"
#include "ui_02_gerenteGUILayouts.hpp"
#include "ui_03_dragDrop.hpp"
#include "source/utils/LogManager.hpp"

#include <QCoreApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSizePolicy>
#include <QVBoxLayout>

LayoutsCompressao::LayoutsCompressao(GerenciadorInterface* gerenciador_interface, CreateButtonFn create_button)
    : m_gerenciador_interface(gerenciador_interface),
      m_create_button(create_button),
      m_max_left_width(0)
{
}

QMap<QString, LayoutsCompressao::LayoutCreatorFn> LayoutsCompressao::create_compression_method_layouts() {
    QMap<QString, LayoutCreatorFn> map;
#if ENABLE_RAR_SUPPORT
    map["rar"] = [this]() { return create_rar_layout(); };
#endif
    map["zip"] = [this]() { return create_zip_layout(); };
    map["7z"] = [this]() { return create_7z_layout(); };
    map["tar"] = [this]() { return create_tar_layout(); };
    map["wim"] = [this]() { return create_wim_layout(); };
    map["extracao"] = [this]() { return create_extracao_layout(); };
    return map;
}

QLayout* LayoutsCompressao::create_layout(const QString& method, const QString& icon_name,
                                          std::function<void()> store_callback,
                                          std::function<void()> clear_callback)
{
    auto layout = new QHBoxLayout();
    auto layout_1 = new QVBoxLayout();
    auto layout_2 = new QVBoxLayout();
    layout_1->insertStretch(0, 1);

    QString base_text = QCoreApplication::translate("InterfaceGrafica", "Especificar Saída");
    QString method_display = method;
    if (method == "extracao") {
        method_display = QCoreApplication::translate("InterfaceGrafica", "Extração");
    } else {
        method_display = QString(".%1").arg(method.toUpper());
    }

    QString output_label_text = QString("%1 %2:").arg(base_text, method_display);

    auto output_hbox = new QHBoxLayout();
    auto output_label = new QLabel(output_label_text);
    output_label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    std::function<void()> output_callback = [this, method]() {
        if (!m_gerenciador_interface) return;
        if (method == "rar") m_gerenciador_interface->output_button_output_RAR_clicked();
        else if (method == "zip") m_gerenciador_interface->output_button_output_ZIP_clicked();
        else if (method == "7z") m_gerenciador_interface->output_button_output_7Z_clicked();
        else if (method == "tar") m_gerenciador_interface->output_button_output_TAR_clicked();
        else if (method == "wim") m_gerenciador_interface->output_button_output_WIM_clicked();
        else if (method == "extracao") m_gerenciador_interface->output_button_output_EXTRACAO_clicked();
    };

    QPushButton* output_button = m_create_button("", output_callback, "saida.png");
    output_hbox->addWidget(output_label);
    output_hbox->addStretch(1);
    output_hbox->addWidget(output_button);
    layout_1->addLayout(output_hbox);

    QString store_text = QCoreApplication::translate("InterfaceGrafica", "Armazenar");
    QString store_label_text;
    if (method == "extracao") {
        store_label_text = QCoreApplication::translate("InterfaceGrafica", "Extrair Arquivos") + ":";
    } else {
        store_label_text = QString("%1 %2:").arg(store_text, method_display);
    }

    auto store_hbox = new QHBoxLayout();
    auto store_label = new QLabel(store_label_text);
    store_label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    QPushButton* store_button = m_create_button("", store_callback, icon_name);
    store_hbox->addWidget(store_label);
    store_hbox->addStretch(1);
    store_hbox->addWidget(store_button);
    layout_1->addLayout(store_hbox);

    QString clear_label_text = QCoreApplication::translate("InterfaceGrafica", "Limpar Saída") + ":";
    auto clear_hbox = new QHBoxLayout();
    auto clear_label = new QLabel(clear_label_text);
    clear_label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    QPushButton* clear_button = m_create_button("", clear_callback, "clear_button2.png");
    clear_hbox->addWidget(clear_label);
    clear_hbox->addStretch(1);
    clear_hbox->addWidget(clear_button);
    layout_1->addLayout(clear_hbox);

    try {
        int left_right_margins = layout_1->contentsMargins().left() + layout_1->contentsMargins().right();
        int spacing_padding = 28;

        struct RowTriple {
            QLabel* lbl;
            QPushButton* btn;
            QHBoxLayout* hbox;
        };
        const RowTriple triples[] = {
            { output_label, output_button, output_hbox },
            { store_label, store_button, store_hbox },
            { clear_label, clear_button, clear_hbox }
        };

        int maxRowW = 0;
        for (const auto& item : triples) {
            int label_w = item.lbl->sizeHint().width();
            int button_w = item.btn->sizeHint().width();
            int hbox_margins = item.hbox->contentsMargins().left() + item.hbox->contentsMargins().right();
            int hbox_spacing = item.hbox->spacing();
            int w = label_w + button_w + left_right_margins + hbox_margins + hbox_spacing + spacing_padding;
            if (w > maxRowW) maxRowW = w;
        }

        if (maxRowW > m_max_left_width) {
            m_max_left_width = maxRowW;
        }

        auto left_widget = new QWidget();
        left_widget->setLayout(layout_1);
        left_widget->setFixedWidth(m_max_left_width);
        left_widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

        m_left_widgets.removeIf([](const QPointer<QWidget>& w) { return w.isNull(); });
        for (const QPointer<QWidget>& w : m_left_widgets) {
            if (w) {
                w->setFixedWidth(m_max_left_width);
                w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
            }
        }

        m_left_widgets.append(left_widget);
        layout->addWidget(left_widget);
        layout->addLayout(layout_2, 1);
    } catch (const std::exception& e) {
        LogManager::error(QString("Erro ao calcular largura mínima do layout_1: %1").arg(e.what()));
        layout->addLayout(layout_1, 1);
        layout->addLayout(layout_2, 3);
    }

    QString output_dir_text = QCoreApplication::translate("InterfaceGrafica", "Diretório(s) de saída");
    QString list_label_text;
    if (method == "extracao") {
        list_label_text = QCoreApplication::translate("InterfaceGrafica", "Diretório(s) de destino para extração:");
    } else {
        list_label_text = QString("%1 %2:").arg(output_dir_text, method_display);
    }

    auto list_label = new QLabel(list_label_text);
    layout_2->addWidget(list_label);

    DragDropListWidget* listbox = nullptr;
    if (m_gerenciador_interface) {
        if (method == "rar") listbox = m_gerenciador_interface->output_listbox_rar;
        else if (method == "zip") listbox = m_gerenciador_interface->output_listbox_zip;
        else if (method == "7z") listbox = m_gerenciador_interface->output_listbox_7z;
        else if (method == "tar") listbox = m_gerenciador_interface->output_listbox_tar;
        else if (method == "wim") listbox = m_gerenciador_interface->output_listbox_wim;
        else if (method == "extracao") listbox = m_gerenciador_interface->output_listbox_extracao;
    }

    if (listbox) {
        listbox->setMinimumHeight(82);
        layout_2->addWidget(listbox);
    }

    return layout;
}

QLayout* LayoutsCompressao::create_rar_layout() {
    return create_layout("rar", "winrar3.png",
                         [this]() { if (m_gerenciador_interface) m_gerenciador_interface->store_as_rar(); },
                         [this]() { if (m_gerenciador_interface) m_gerenciador_interface->clear_output_listbox_rar(); });
}

QLayout* LayoutsCompressao::create_zip_layout() {
    return create_layout("zip", "winzip4.png",
                         [this]() { if (m_gerenciador_interface) m_gerenciador_interface->store_as_zip(); },
                         [this]() { if (m_gerenciador_interface) m_gerenciador_interface->clear_output_listbox_zip(); });
}

QLayout* LayoutsCompressao::create_7z_layout() {
    return create_layout("7z", "sevenzip3.png",
                         [this]() { if (m_gerenciador_interface) m_gerenciador_interface->store_as_7z(); },
                         [this]() { if (m_gerenciador_interface) m_gerenciador_interface->clear_output_listbox_7z(); });
}

QLayout* LayoutsCompressao::create_tar_layout() {
    return create_layout("tar", "tar1.png",
                         [this]() { if (m_gerenciador_interface) m_gerenciador_interface->store_as_tar(); },
                         [this]() { if (m_gerenciador_interface) m_gerenciador_interface->clear_output_listbox_tar(); });
}

QLayout* LayoutsCompressao::create_wim_layout() {
    return create_layout("wim", "wim.png",
                         [this]() { if (m_gerenciador_interface) m_gerenciador_interface->store_as_wim(); },
                         [this]() { if (m_gerenciador_interface) m_gerenciador_interface->clear_output_listbox_wim(); });
}

QLayout* LayoutsCompressao::create_extracao_layout() {
    return create_layout("extracao", "extracao4.png",
                         [this]() { if (m_gerenciador_interface) m_gerenciador_interface->extract_files(); },
                         [this]() { if (m_gerenciador_interface) m_gerenciador_interface->clear_output_listbox_extracao(); });
}

void LayoutsCompressao::clear_left_widgets() {
    m_left_widgets.clear();
    m_max_left_width = 0;
}
