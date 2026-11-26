#pragma once

#include <QThread>
#include <QString>
#include <QStringList>

class DragDropListWidget;

QString buscar_executavel(const QString& nome_pasta, const QString& nome_executavel, const QStringList& possiveis_caminhos);
QString buscar_winrar_executavel();
QString buscar_sevenzip_executavel();
QString buscar_diretorio_plugins_7z();
QString buscar_diretorio_plugins_winrar();
QString buscar_arquivo_plugin(const QString& nome_pasta, const QString& nome_arquivo);

class CompressaoBase : public QThread {
    Q_OBJECT

public:
    CompressaoBase(const QString& executable,
                   bool update_existing,
                   DragDropListWidget* output_listbox,
                   DragDropListWidget* folder_listbox,
                   bool compress_as = false,
                   const QString& compression_method = QString(),
                   QObject* parent = nullptr);

    ~CompressaoBase() override = default;

    QString executable;
    bool update_existing = false;
    DragDropListWidget* output_listbox = nullptr;
    DragDropListWidget* folder_listbox = nullptr;
    bool compress_as = false;
    QString compression_method;
    QString format;

    virtual void comprimir(const QString& output_path, const QString& folder_path, const QString& folder_name) = 0;

protected:
    void run() override;

signals:
    void finished();
};

class CompressaoRAR : public CompressaoBase {
    Q_OBJECT
public:
    using CompressaoBase::CompressaoBase;
    CompressaoRAR(const QString& executable,
                  bool update_existing,
                  DragDropListWidget* output_listbox,
                  DragDropListWidget* folder_listbox,
                  bool compress_as = false,
                  const QString& compression_method = QString(),
                  QObject* parent = nullptr);
    void comprimir(const QString& output_path, const QString& folder_path, const QString& folder_name) override;
};

class CompressaoZIP : public CompressaoBase {
    Q_OBJECT
public:
    using CompressaoBase::CompressaoBase;
    CompressaoZIP(const QString& executable,
                  bool update_existing,
                  DragDropListWidget* output_listbox,
                  DragDropListWidget* folder_listbox,
                  bool compress_as = false,
                  const QString& compression_method = QString(),
                  QObject* parent = nullptr);
    void comprimir(const QString& output_path, const QString& folder_path, const QString& folder_name) override;
};

class Compressao7Z : public CompressaoBase {
    Q_OBJECT
public:
    using CompressaoBase::CompressaoBase;
    Compressao7Z(const QString& executable,
                 bool update_existing,
                 DragDropListWidget* output_listbox,
                 DragDropListWidget* folder_listbox,
                 bool compress_as = false,
                 const QString& compression_method = QString(),
                 QObject* parent = nullptr);
    void comprimir(const QString& output_path, const QString& folder_path, const QString& folder_name) override;
};

class CompressaoTAR : public CompressaoBase {
    Q_OBJECT
public:
    using CompressaoBase::CompressaoBase;
    CompressaoTAR(const QString& executable,
                  bool update_existing,
                  DragDropListWidget* output_listbox,
                  DragDropListWidget* folder_listbox,
                  bool compress_as = false,
                  const QString& compression_method = QString(),
                  QObject* parent = nullptr);
    void comprimir(const QString& output_path, const QString& folder_path, const QString& folder_name) override;
};

class CompressaoWIM : public CompressaoBase {
    Q_OBJECT
public:
    using CompressaoBase::CompressaoBase;
    CompressaoWIM(const QString& executable,
                  bool update_existing,
                  DragDropListWidget* output_listbox,
                  DragDropListWidget* folder_listbox,
                  bool compress_as = false,
                  const QString& compression_method = QString(),
                  QObject* parent = nullptr);
    void comprimir(const QString& output_path, const QString& folder_path, const QString& folder_name) override;
};

class TesteIntegridade : public QThread {
    Q_OBJECT

public:
    TesteIntegridade(const QString& sevenzip_executable, const QStringList& compressed_files, QObject* parent = nullptr);
    ~TesteIntegridade() override = default;

protected:
    void run() override;

signals:
    void finished();

private:
    QString sevenzip_executable;
    QStringList compressed_files;
};

class Extracao : public QThread {
    Q_OBJECT

public:
    Extracao(const QString& sevenzip_executable,
             DragDropListWidget* output_listbox,
             DragDropListWidget* folder_listbox,
             QObject* parent = nullptr);
    ~Extracao() override = default;

    void extrair(const QString& archive_path, const QString& output_path);

protected:
    void run() override;

signals:
    void finished();

private:
    QString sevenzip_executable;
    DragDropListWidget* output_listbox = nullptr;
    DragDropListWidget* folder_listbox = nullptr;
};
