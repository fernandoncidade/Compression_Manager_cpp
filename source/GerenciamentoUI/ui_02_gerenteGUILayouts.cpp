#include "ui_02_gerenteGUILayouts.hpp"
#include "ui_03_dragDrop.hpp"
#include "ui_04_dialogTraducao.hpp"
#include "source/MotoresCompressao/mtcomp_02_motoresCompressao.hpp"
#include "source/utils/LogManager.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QTreeView>

GerenciadorInterface::GerenciadorInterface(QWidget* parent)
    : QMainWindow(parent)
{
    winrar_executavel = buscar_winrar_executavel();
    sevenzip_executavel = buscar_sevenzip_executavel();

    if (sevenzip_executavel.isEmpty() && winrar_executavel.isEmpty()) {
        LogManager::warning("7-Zip e WinRAR não encontrados. Por favor, instale um deles e tente novamente.");
    }

    output_listbox_rar = new DragDropListWidget(this, true);
    output_listbox_zip = new DragDropListWidget(this, true);
    output_listbox_7z = new DragDropListWidget(this, true);
    output_listbox_tar = new DragDropListWidget(this, true);
    output_listbox_wim = new DragDropListWidget(this, true);
    output_listbox = new DragDropListWidget(this, true);
    folder_listbox = new DragDropListWidget(this, false);
    output_listbox_extracao = new DragDropListWidget(this, true);

    init_threads();
}

GerenciadorInterface::~GerenciadorInterface() {
    for (CompressaoBase* t : compress_threads) {
        if (t && t->isRunning()) {
            t->terminate();
            t->wait();
        }
    }
}

void GerenciadorInterface::init_threads() {
    compress_thread_rar = new CompressaoRAR(winrar_executavel, update_existing, output_listbox, folder_listbox, false, QString(), this);
    connect(compress_thread_rar, &CompressaoBase::finished, this, &GerenciadorInterface::on_compress_finished);

    compress_thread_zip = new CompressaoZIP(sevenzip_executavel, update_existing, output_listbox, folder_listbox, false, QString(), this);
    connect(compress_thread_zip, &CompressaoBase::finished, this, &GerenciadorInterface::on_compress_finished);

    compress_thread_7z = new Compressao7Z(sevenzip_executavel, update_existing, output_listbox, folder_listbox, false, QString(), this);
    connect(compress_thread_7z, &CompressaoBase::finished, this, &GerenciadorInterface::on_compress_finished);

    compress_thread_tar = new CompressaoTAR(sevenzip_executavel, update_existing, output_listbox, folder_listbox, false, QString(), this);
    connect(compress_thread_tar, &CompressaoBase::finished, this, &GerenciadorInterface::on_compress_finished);

    compress_thread_wim = new CompressaoWIM(sevenzip_executavel, update_existing, output_listbox, folder_listbox, false, QString(), this);
    connect(compress_thread_wim, &CompressaoBase::finished, this, &GerenciadorInterface::on_compress_finished);

    teste_integridade_thread = new TesteIntegridade(sevenzip_executavel, compressed_files, this);
    connect(teste_integridade_thread, &TesteIntegridade::finished, this, &GerenciadorInterface::on_teste_integridade_finished);

    extract_thread = new Extracao(sevenzip_executavel, output_listbox, folder_listbox, this);
    connect(extract_thread, &Extracao::finished, this, &GerenciadorInterface::on_extract_finished);
}

void GerenciadorInterface::browse_folder() {
    _browse(QFileDialog::Directory, { folder_listbox });
}

void GerenciadorInterface::browse_file() {
    _browse(QFileDialog::ExistingFiles, { folder_listbox });
}

void GerenciadorInterface::browse_output() {
    _browse(QFileDialog::Directory, { output_listbox, output_listbox_zip, output_listbox_7z, output_listbox_tar, output_listbox_wim, output_listbox_extracao });
}

void GerenciadorInterface::_browse(QFileDialog::FileMode file_mode, const QList<DragDropListWidget*>& target_listboxes) {
    if (file_mode == QFileDialog::Directory) {
        FileDialogTraduzivel dialog(this);
        dialog.setFileMode(file_mode);
        dialog.setOption(QFileDialog::DontUseNativeDialog, false);
        dialog.setOption(QFileDialog::ShowDirsOnly, true);
        dialog.traduzir_botoes();

        if (!last_directory.isEmpty() && QDir(last_directory).exists()) {
            dialog.setDirectory(last_directory);
        }

        bool continue_selecting = true;
        bool first_selection = true;

        while (continue_selecting) {
            QString dialog_title = first_selection 
                ? QCoreApplication::translate("InterfaceGrafica", "Selecione um diretório")
                : QCoreApplication::translate("InterfaceGrafica", "Selecione outro diretório");
            dialog.setWindowTitle(dialog_title);

            if (dialog.exec() == QFileDialog::Accepted) {
                QStringList selected_dirs = dialog.selectedFiles();
                if (!selected_dirs.isEmpty()) {
                    for (DragDropListWidget* lb : target_listboxes) {
                        if (!lb) continue;
                        for (const QString& dir_path : selected_dirs) {
                            bool exists = false;
                            for (int i = 0; i < lb->count(); ++i) {
                                if (lb->item(i)->text() == dir_path) {
                                    exists = true;
                                    break;
                                }
                            }
                            if (!exists) {
                                lb->addItem(dir_path);
                            }
                        }
                    }
                    last_directory = QFileInfo(selected_dirs.first()).absolutePath();
                }

                QCoreApplication::processEvents();

                auto msg_box = new MessageBoxTraduzivel(
                    QMessageBox::Question,
                    QCoreApplication::translate("InterfaceGrafica", "Seleção Múltipla"),
                    QCoreApplication::translate("InterfaceGrafica", "Deseja adicionar mais diretórios?"),
                    QMessageBox::Yes | QMessageBox::No,
                    this
                );

                dialogos_ativos.append(msg_box);
                connect(msg_box, &QMessageBox::finished, [this, msg_box]() {
                    dialogos_ativos.removeOne(msg_box);
                    msg_box->deleteLater();
                });

                msg_box->setWindowModality(Qt::ApplicationModal);
                msg_box->setWindowFlag(Qt::WindowStaysOnTopHint, true);
                msg_box->raise();
                msg_box->activateWindow();

                int reply = msg_box->exec();
                continue_selecting = (reply == QMessageBox::Yes);
                first_selection = false;

                if (continue_selecting && !last_directory.isEmpty()) {
                    dialog.setDirectory(last_directory);
                    QCoreApplication::processEvents();
                }
            } else {
                continue_selecting = false;
            }
        }
        return;
    }

    FileDialogTraduzivel dialog(this);
    dialog.setFileMode(file_mode);
    dialog.traduzir_botoes();

    if (file_mode == QFileDialog::ExistingFiles) {
        dialog.setWindowTitle(QCoreApplication::translate("InterfaceGrafica", "Selecionar Arquivos"));
    } else {
        dialog.setWindowTitle(QCoreApplication::translate("InterfaceGrafica", "Selecionar"));
    }

    if (!last_directory.isEmpty() && QDir(last_directory).exists()) {
        dialog.setDirectory(last_directory);
    }

    auto tree_view = dialog.findChild<QTreeView*>();
    if (tree_view) {
        tree_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }

    if (dialog.exec() == QFileDialog::Accepted) {
        QStringList selected_items = dialog.selectedFiles();
        if (!selected_items.isEmpty()) {
            for (DragDropListWidget* lb : target_listboxes) {
                if (!lb) continue;
                for (const QString& item : selected_items) {
                    bool exists = false;
                    for (int i = 0; i < lb->count(); ++i) {
                        if (lb->item(i)->text() == item) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        lb->addItem(item);
                    }
                }
            }
            last_directory = QFileInfo(selected_items.first()).absolutePath();
        }
    }
}

void GerenciadorInterface::select_output_path(DragDropListWidget* output_lb) {
    _browse(QFileDialog::Directory, { output_lb });
}

void GerenciadorInterface::output_button_output_RAR_clicked() {
    select_output_path(output_listbox_rar);
}

void GerenciadorInterface::output_button_output_ZIP_clicked() {
    select_output_path(output_listbox_zip);
}

void GerenciadorInterface::output_button_output_7Z_clicked() {
    select_output_path(output_listbox_7z);
}

void GerenciadorInterface::output_button_output_TAR_clicked() {
    select_output_path(output_listbox_tar);
}

void GerenciadorInterface::output_button_output_WIM_clicked() {
    select_output_path(output_listbox_wim);
}

void GerenciadorInterface::output_button_output_EXTRACAO_clicked() {
    select_output_path(output_listbox_extracao);
}

void GerenciadorInterface::clear_folders() {
    if (folder_listbox) folder_listbox->clear();
}

void GerenciadorInterface::clear_output() {
    if (output_listbox) output_listbox->clear();
    if (output_listbox_rar) output_listbox_rar->clear();
    if (output_listbox_zip) output_listbox_zip->clear();
    if (output_listbox_7z) output_listbox_7z->clear();
    if (output_listbox_tar) output_listbox_tar->clear();
    if (output_listbox_wim) output_listbox_wim->clear();
    if (output_listbox_extracao) output_listbox_extracao->clear();
}

void GerenciadorInterface::clear_output_listbox_rar() {
    if (output_listbox_rar) output_listbox_rar->clear();
}

void GerenciadorInterface::clear_output_listbox_zip() {
    if (output_listbox_zip) output_listbox_zip->clear();
}

void GerenciadorInterface::clear_output_listbox_7z() {
    if (output_listbox_7z) output_listbox_7z->clear();
}

void GerenciadorInterface::clear_output_listbox_tar() {
    if (output_listbox_tar) output_listbox_tar->clear();
}

void GerenciadorInterface::clear_output_listbox_wim() {
    if (output_listbox_wim) output_listbox_wim->clear();
}

void GerenciadorInterface::clear_output_listbox_extracao() {
    if (output_listbox_extracao) output_listbox_extracao->clear();
}

void GerenciadorInterface::store_as_rar() {
    _store_as("rar", output_listbox_rar, compression_method_rar);
}

void GerenciadorInterface::store_as_zip() {
    _store_as("zip", output_listbox_zip, compression_method_zip);
}

void GerenciadorInterface::store_as_7z() {
    _store_as("7z", output_listbox_7z, compression_method_7z);
}

void GerenciadorInterface::store_as_tar() {
    _store_as("tar", output_listbox_tar, compression_method_tar);
}

void GerenciadorInterface::store_as_wim() {
    _store_as("wim", output_listbox_wim, compression_method_wim);
}

void GerenciadorInterface::_store_as(const QString& format, DragDropListWidget* target_output_lb, const QString& compression_method) {
    if (!folder_listbox || folder_listbox->count() == 0) {
        show_selection_compression_warning();
    } else if (!target_output_lb || target_output_lb->count() == 0) {
        show_selection_destination_warning();
    } else if (!compression_method.isEmpty()) {
        CompressaoBase* new_thread = nullptr;
        if (format == "rar") {
            new_thread = new CompressaoRAR(winrar_executavel, update_existing, target_output_lb, folder_listbox, true, compression_method, this);
        } else if (format == "zip") {
            new_thread = new CompressaoZIP(sevenzip_executavel, update_existing, target_output_lb, folder_listbox, true, compression_method, this);
        } else if (format == "7z") {
            new_thread = new Compressao7Z(sevenzip_executavel, update_existing, target_output_lb, folder_listbox, true, compression_method, this);
        } else if (format == "tar") {
            new_thread = new CompressaoTAR(sevenzip_executavel, update_existing, target_output_lb, folder_listbox, true, compression_method, this);
        } else if (format == "wim") {
            new_thread = new CompressaoWIM(sevenzip_executavel, update_existing, target_output_lb, folder_listbox, true, compression_method, this);
        }

        if (new_thread) {
            connect(new_thread, &CompressaoBase::finished, this, &GerenciadorInterface::on_compress_finished);
            start_compression_thread(new_thread);
        }
    } else {
        show_method_warning();
    }
}

void GerenciadorInterface::testar_integridade() {
    if (!folder_listbox) return;

    QStringList files;
    for (int i = 0; i < folder_listbox->count(); ++i) {
        files << folder_listbox->item(i)->text();
    }

    const QStringList supportedExts = {
        ".rar", ".zip", ".7z", ".bz2", ".tar", ".wim", ".tar.xz", ".zipx", ".tgz", ".tar.gz", ".lzh", ".iso"
    };

    QStringList matched;
    for (const QString& f : files) {
        QString lower = f.toLower();
        for (const QString& ext : supportedExts) {
            if (lower.endsWith(ext)) {
                matched << f;
                break;
            }
        }
    }

    if (matched.isEmpty()) {
        show_extension_warning();
        return;
    }

    if (!sevenzip_executavel.isEmpty()) {
        auto thread = new TesteIntegridade(sevenzip_executavel, matched, this);
        connect(thread, &TesteIntegridade::finished, this, &GerenciadorInterface::on_teste_integridade_finished);
        connect(thread, &TesteIntegridade::finished, thread, &QObject::deleteLater);
        thread->start();
    } else {
        show_extract_warning();
    }
}

void GerenciadorInterface::extract_files() {
    if (!folder_listbox || folder_listbox->count() == 0) {
        show_selection_descompression_warning();
    } else if (!output_listbox_extracao || output_listbox_extracao->count() == 0) {
        show_selection_destination_warning();
    } else {
        _start_extraction_thread();
    }
}

void GerenciadorInterface::_start_extraction_thread() {
    if (sevenzip_executavel.isEmpty()) {
        show_extract_warning();
        return;
    }

    auto thread = new Extracao(sevenzip_executavel, output_listbox_extracao, folder_listbox, this);
    connect(thread, &Extracao::finished, this, &GerenciadorInterface::on_extract_finished);
    connect(thread, &Extracao::finished, thread, &QObject::deleteLater);
    thread->start();
}

void GerenciadorInterface::start_compression_thread(CompressaoBase* new_thread) {
    if (!new_thread) return;

    QString new_format = new_thread->format;
    bool alreadyRunning = false;
    for (CompressaoBase* t : compress_threads) {
        if (t && t->isRunning() && t->format == new_format) {
            alreadyRunning = true;
            break;
        }
    }

    if (alreadyRunning) {
        compress_queue.enqueue(new_thread);
        show_queue_warning();
    } else {
        compress_threads.append(new_thread);
        new_thread->start();
    }
}

void GerenciadorInterface::on_compress_finished() {
    auto finished_thread = qobject_cast<CompressaoBase*>(sender());
    if (finished_thread) {
        compress_threads.removeOne(finished_thread);
        finished_thread->deleteLater();
    }

    if (!compress_queue.isEmpty()) {
        CompressaoBase* next_thread = compress_queue.dequeue();
        start_compression_thread(next_thread);
    } else if (compress_threads.isEmpty()) {
        show_info_message(
            QCoreApplication::translate("InterfaceGrafica", "Empacotamento Concluído"),
            QCoreApplication::translate("InterfaceGrafica", "O Empacotamento dos arquivos foi concluído com sucesso!")
        );
    }
}

void GerenciadorInterface::on_teste_integridade_finished() {
    show_info_message(
        QCoreApplication::translate("InterfaceGrafica", "Teste de Integridade Concluído"),
        QCoreApplication::translate("InterfaceGrafica", "O teste de integridade dos arquivos foi concluído com sucesso!")
    );
}

void GerenciadorInterface::on_extract_finished() {
    show_info_message(
        QCoreApplication::translate("InterfaceGrafica", "Extração Concluída"),
        QCoreApplication::translate("InterfaceGrafica", "A Extração dos arquivos foi concluída com sucesso!")
    );
}

void GerenciadorInterface::show_queue_warning() {
    show_warning(
        QCoreApplication::translate("InterfaceGrafica", "Aviso"),
        QCoreApplication::translate("InterfaceGrafica", "O processo solicitado foi colocado em fila, aguardando o anterior encerrar.")
    );
}

void GerenciadorInterface::show_method_warning() {
    show_warning(
        QCoreApplication::translate("InterfaceGrafica", "Aviso"),
        QCoreApplication::translate("InterfaceGrafica", "Por favor, selecione um método de compressão antes de prosseguir.")
    );
}

void GerenciadorInterface::show_integridade_warning() {
    show_warning(
        QCoreApplication::translate("InterfaceGrafica", "Aviso"),
        QCoreApplication::translate("InterfaceGrafica", "Por favor, selecione um arquivo para testar a integridade.")
    );
}

void GerenciadorInterface::show_extension_warning() {
    show_warning(
        QCoreApplication::translate("InterfaceGrafica", "Aviso"),
        QCoreApplication::translate("InterfaceGrafica", "Por favor, selecione um arquivo COMPACTADO para prosseguir.")
    );
}

void GerenciadorInterface::show_extract_warning() {
    show_warning(
        QCoreApplication::translate("InterfaceGrafica", "Aviso"),
        QCoreApplication::translate("InterfaceGrafica", "7-Zip não encontrado. Por favor, instale o 7-Zip e tente novamente.")
    );
}

void GerenciadorInterface::show_selection_compression_warning() {
    show_warning(
        QCoreApplication::translate("InterfaceGrafica", "Aviso"),
        QCoreApplication::translate("InterfaceGrafica", "Por favor, selecione uma pasta antes de prosseguir.")
    );
}

void GerenciadorInterface::show_selection_descompression_warning() {
    show_warning(
        QCoreApplication::translate("InterfaceGrafica", "Aviso"),
        QCoreApplication::translate("InterfaceGrafica", "Por favor, selecione um arquivo antes de prosseguir.")
    );
}

void GerenciadorInterface::show_selection_destination_warning() {
    show_warning(
        QCoreApplication::translate("InterfaceGrafica", "Aviso"),
        QCoreApplication::translate("InterfaceGrafica", "Por favor, selecione um diretório de saída antes de prosseguir.")
    );
}

void GerenciadorInterface::show_warning(const QString& title, const QString& message) {
    show_message(QMessageBox::Warning, title, message);
}

void GerenciadorInterface::show_info_message(const QString& title, const QString& message) {
    show_message(QMessageBox::Information, title, message);
}

void GerenciadorInterface::atualizar_traducoes_dialogos() {
    for (QWidget* dialogo : dialogos_ativos) {
        if (auto mb = qobject_cast<MessageBoxTraduzivel*>(dialogo)) {
            mb->atualizar_traducao();
        }
    }
}

void GerenciadorInterface::show_message(QMessageBox::Icon icon, const QString& title, const QString& message) {
    QCoreApplication::processEvents();
    auto msg_box = new MessageBoxTraduzivel(icon, title, message, QMessageBox::Ok, this);
    dialogos_ativos.append(msg_box);
    connect(msg_box, &QMessageBox::finished, [this, msg_box]() {
        dialogos_ativos.removeOne(msg_box);
        msg_box->deleteLater();
    });
    msg_box->setWindowModality(Qt::ApplicationModal);
    msg_box->setWindowFlag(Qt::WindowStaysOnTopHint, true);
    msg_box->raise();
    msg_box->activateWindow();
    msg_box->exec();
}

QString GerenciadorInterface::getCompressionMethod(const QString& type) const {
    if (type == "rar") return compression_method_rar;
    if (type == "zip") return compression_method_zip;
    if (type == "7z") return compression_method_7z;
    if (type == "tar") return compression_method_tar;
    if (type == "wim") return compression_method_wim;
    return QString();
}

void GerenciadorInterface::setCompressionMethod(const QString& type, const QString& method) {
    if (type == "rar") compression_method_rar = method;
    else if (type == "zip") compression_method_zip = method;
    else if (type == "7z") compression_method_7z = method;
    else if (type == "tar") compression_method_tar = method;
    else if (type == "wim") compression_method_wim = method;
}
