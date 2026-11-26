#pragma once

#include <QObject>
#include <QString>
#include <QMap>
#include <QTranslator>
#include <memory>

class QDialog;

class GerenciadorTraducao : public QObject {
    Q_OBJECT

public:
    explicit GerenciadorTraducao(QObject* parent = nullptr);
    ~GerenciadorTraducao() override;

    void carregar_configuracao_idioma();
    void salvar_configuracao_idioma();
    QString obter_caminho_configuracao();

    bool aplicar_traducao();
    bool definir_idioma(const QString& codigo_idioma);
    QString obter_idioma_atual() const;
    QTranslator* obter_tradutor() const { return tradutor.get(); }

    static void traduzir_botoes_padrao(QDialog* dialogo);

    QString idioma_atual;
    QString idioma_padrao;
    QMap<QString, QString> idiomas_disponiveis;
    QString dir_traducoes;
    std::unique_ptr<QTranslator> tradutor;

signals:
    void idioma_alterado(const QString& codigo_idioma);
};
