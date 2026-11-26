#include "test_common.hpp"

#include "source/GerenciamentoUI/ui_04_dialogTraducao.hpp"
#include "source/language/tr_01_gerenciadorTraducao.hpp"
#include "source/language/tr_02_compileTranslations.hpp"
#include "source/utils/CaminhoPersistenteUtils.hpp"

#include <QAbstractButton>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QGroupBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QProcess>
#include <QPushButton>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QTranslator>

class TestI18nTranslations : public QObject {
    Q_OBJECT

private slots:
    void testTranslatorLifecyclePersistenceAndSignals() {
        QFile::remove(QDir(obter_caminho_persistente()).filePath("language.json"));

        GerenciadorTraducao gt;
        QCOMPARE(gt.obter_idioma_atual(), QStringLiteral("pt_BR"));
        QVERIFY(gt.idiomas_disponiveis.contains("pt_BR"));
        QVERIFY(gt.idiomas_disponiveis.contains("en_US"));
        QVERIFY(QFileInfo::exists(QDir(gt.dir_traducoes).filePath("manager_compression_en_US.qm")));
        QVERIFY(QFileInfo::exists(QDir(gt.dir_traducoes).filePath("manager_compression_pt_BR.qm")));

        // Idioma invalido nao altera estado nem emite sinal
        QSignalSpy spy(&gt, &GerenciadorTraducao::idioma_alterado);
        QVERIFY(!gt.definir_idioma("fr_FR"));
        QCOMPARE(spy.count(), 0);
        QCOMPARE(gt.obter_idioma_atual(), QStringLiteral("pt_BR"));

        // Troca para en_US e verifica traducao real de strings
        QVERIFY(gt.definir_idioma("en_US"));
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.takeFirst().at(0).toString(), QStringLiteral("en_US"));
        QCOMPARE(gt.obter_idioma_atual(), QStringLiteral("en_US"));
        QVERIFY(gt.obter_tradutor() != nullptr);

        QCOMPARE(QCoreApplication::translate("InterfaceGrafica", "Arquivo"), QStringLiteral("File"));
        QCOMPARE(QCoreApplication::translate("InterfaceGrafica", "EXTRAÇÃO"), QStringLiteral("EXTRACTION"));
        QCOMPARE(QCoreApplication::translate("CompressLevels", "Armazenar"), QStringLiteral("Store"));
        QCOMPARE(QCoreApplication::translate("CompressLevels", "Ultra"), QStringLiteral("Ultra"));

        // Verifica persistencia em language.json e recarga em nova instancia
        const QString cfgPath = gt.obter_caminho_configuracao();
        QVERIFY(QFileInfo::exists(cfgPath));
        {
            GerenciadorTraducao gtReloaded;
            QCOMPARE(gtReloaded.obter_idioma_atual(), QStringLiteral("en_US"));
            QVERIFY(gtReloaded.aplicar_traducao());
            QCOMPARE(QCoreApplication::translate("InterfaceGrafica", "Arquivo"), QStringLiteral("File"));
        }

        // Restaura pt_BR
        QVERIFY(gt.definir_idioma("pt_BR"));
        QCOMPARE(gt.obter_idioma_atual(), QStringLiteral("pt_BR"));
        QCOMPARE(QCoreApplication::translate("InterfaceGrafica", "Arquivo"), QStringLiteral("Arquivo"));

        // Resiliencia com JSON corrompido
        QVERIFY(cm_test::writeBytes(cfgPath, QByteArray("{corrupt_json")));
        GerenciadorTraducao gtCorrupt;
        QCOMPARE(gtCorrupt.obter_idioma_atual(), QStringLiteral("pt_BR"));
    }

    void testTranslatableDialogsAndButtons() {
        QFile::remove(QDir(obter_caminho_persistente()).filePath("language.json"));
        GerenciadorTraducao gt;

        // DialogoTraduzivel
        DialogoTraduzivel dlg;
        auto* lbl = new QLabel(&dlg);
        auto* btn = new QPushButton(&dlg);
        auto* grp = new QGroupBox(&dlg);
        dlg.adicionar_texto_traduzir(lbl, "Arquivo", "InterfaceGrafica");
        dlg.adicionar_texto_traduzir(btn, "Sair", "InterfaceGrafica");
        dlg.adicionar_texto_traduzir(grp, "Opções", "InterfaceGrafica");

        QVERIFY(gt.definir_idioma("en_US"));
        dlg.traduzir_interface();
        QCOMPARE(lbl->text(), QStringLiteral("File"));
        QCOMPARE(btn->text(), QStringLiteral("Exit"));
        QCOMPARE(grp->title(), QStringLiteral("Options"));

        // FileDialogTraduzivel
        FileDialogTraduzivel fileDlg;
        fileDlg.traduzir_botoes();
        QCOMPARE(fileDlg.labelText(QFileDialog::Accept), QCoreApplication::translate("FileDialog", "Abrir"));
        QCOMPARE(fileDlg.labelText(QFileDialog::Reject), QCoreApplication::translate("FileDialog", "Cancelar"));

        // MessageBoxTraduzivel
        MessageBoxTraduzivel msgBox(QMessageBox::Question, "Aviso", "Deseja adicionar mais diretórios?",
                                    QMessageBox::Yes | QMessageBox::No);
        msgBox.atualizar_traducao();
        QCOMPARE(msgBox.windowTitle(), QCoreApplication::translate("InterfaceGrafica", "Aviso"));
        QCOMPARE(msgBox.text(), QCoreApplication::translate("InterfaceGrafica", "Deseja adicionar mais diretórios?"));
        QVERIFY(msgBox.button(QMessageBox::Yes) != nullptr);
        QCOMPARE(msgBox.button(QMessageBox::Yes)->text(), QCoreApplication::translate("Dialog", "Sim"));
        QCOMPARE(msgBox.button(QMessageBox::No)->text(), QCoreApplication::translate("Dialog", "Não"));

        // Volta para pt_BR
        QVERIFY(gt.definir_idioma("pt_BR"));
        dlg.traduzir_interface();
        msgBox.atualizar_traducao();
        QCOMPARE(lbl->text(), QStringLiteral("Arquivo"));
        QCOMPARE(msgBox.button(QMessageBox::Yes)->text(), QStringLiteral("Sim"));
    }

    void testIsolatedLreleaseCompilationToTempDir() {
        const QString lreleaseExe = TranslationCompiler::encontrar_lrelease();
        QVERIFY2(!lreleaseExe.isEmpty(), "encontrar_lrelease() returned empty");

        QTemporaryDir tempOut;
        QVERIFY(tempOut.isValid());

        const QString tsDir = QDir(cm_test::sourceRootDir()).filePath("source/language/translations");
        QVERIFY(QDir(tsDir).exists());

        for (const QString& lang : { "pt_BR", "en_US" }) {
            const QString tsFile = QDir(tsDir).filePath(QString("manager_compression_%1.ts").arg(lang));
            const QString qmOut = QDir(tempOut.path()).filePath(QString("manager_compression_%1.qm").arg(lang));
            QVERIFY2(QFileInfo::exists(tsFile), qPrintable("Missing .ts source: " + tsFile));

            QProcess proc;
            proc.start(lreleaseExe, { tsFile, "-qm", qmOut });
            QVERIFY2(proc.waitForStarted(5000), qPrintable("Failed to start lrelease: " + lreleaseExe));
            QVERIFY2(proc.waitForFinished(15000), "lrelease timed out");
            QCOMPARE(proc.exitCode(), 0);
            QVERIFY(QFileInfo::exists(qmOut) && QFileInfo(qmOut).size() > 0);

            QTranslator probe;
            QVERIFY2(probe.load(qmOut), qPrintable("Compiled .qm invalid: " + qmOut));
        }
    }
};

CM_TEST_MAIN(TestI18nTranslations)
#ifndef __INTELLISENSE__
#include "test_03_i18n_translations.moc"
#endif
