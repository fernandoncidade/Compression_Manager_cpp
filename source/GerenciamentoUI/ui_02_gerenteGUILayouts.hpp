#pragma once

#include <QMainWindow>
#include <QQueue>
#include <QList>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>

class DragDropListWidget;
class CompressaoBase;
class CompressaoRAR;
class CompressaoZIP;
class Compressao7Z;
class CompressaoTAR;
class CompressaoWIM;
class TesteIntegridade;
class Extracao;

class GerenciadorInterface : public QMainWindow {
    Q_OBJECT

public:
    explicit GerenciadorInterface(QWidget* parent = nullptr);
    ~GerenciadorInterface() override;

    void init_threads();

    void browse_folder();
    void browse_file();
    void browse_output();
    void _browse(QFileDialog::FileMode file_mode, const QList<DragDropListWidget*>& listboxes);
    void select_output_path(DragDropListWidget* output_listbox);

    void output_button_output_RAR_clicked();
    void output_button_output_ZIP_clicked();
    void output_button_output_7Z_clicked();
    void output_button_output_TAR_clicked();
    void output_button_output_WIM_clicked();
    void output_button_output_EXTRACAO_clicked();

    void clear_folders();
    void clear_output();
    void clear_output_listbox_rar();
    void clear_output_listbox_zip();
    void clear_output_listbox_7z();
    void clear_output_listbox_tar();
    void clear_output_listbox_wim();
    void clear_output_listbox_extracao();

    void store_as_rar();
    void store_as_zip();
    void store_as_7z();
    void store_as_tar();
    void store_as_wim();
    void _store_as(const QString& format, DragDropListWidget* output_listbox, const QString& compression_method);

    void testar_integridade();
    void extract_files();
    void _start_extraction_thread();

    void start_compression_thread(CompressaoBase* new_thread);

    void show_queue_warning();
    void show_method_warning();
    void show_integridade_warning();
    void show_extension_warning();
    void show_extract_warning();
    void show_selection_compression_warning();
    void show_selection_descompression_warning();
    void show_selection_destination_warning();
    void show_warning(const QString& title, const QString& message);
    void show_info_message(const QString& title, const QString& message);
    void show_message(QMessageBox::Icon icon, const QString& title, const QString& message);
    void atualizar_traducoes_dialogos();

    QString getCompressionMethod(const QString& type) const;
    void setCompressionMethod(const QString& type, const QString& method);

public slots:
    void on_compress_finished();
    void on_teste_integridade_finished();
    void on_extract_finished();

public:
    QQueue<CompressaoBase*> compress_queue;
    QList<CompressaoBase*> compress_threads;
    QList<QWidget*> dialogos_ativos;

    QString compression_method_rar;
    QString compression_method_zip;
    QString compression_method_7z;
    QString compression_method_tar;
    QString compression_method_wim;
    bool update_existing = false;
    QString last_directory;

    QString winrar_executavel;
    QString sevenzip_executavel;

    DragDropListWidget* output_listbox_rar = nullptr;
    DragDropListWidget* output_listbox_zip = nullptr;
    DragDropListWidget* output_listbox_7z = nullptr;
    DragDropListWidget* output_listbox_tar = nullptr;
    DragDropListWidget* output_listbox_wim = nullptr;
    DragDropListWidget* output_listbox = nullptr;
    DragDropListWidget* folder_listbox = nullptr;
    DragDropListWidget* output_listbox_extracao = nullptr;

    QStringList compressed_files;

    CompressaoRAR* compress_thread_rar = nullptr;
    CompressaoZIP* compress_thread_zip = nullptr;
    Compressao7Z* compress_thread_7z = nullptr;
    CompressaoTAR* compress_thread_tar = nullptr;
    CompressaoWIM* compress_thread_wim = nullptr;
    TesteIntegridade* teste_integridade_thread = nullptr;
    Extracao* extract_thread = nullptr;
};
