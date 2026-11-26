#include "test_common.hpp"

#include "source/GerenciamentoUI/ui_02_gerenteGUILayouts.hpp"
#include "source/MotoresCompressao/mtcomp_01_metodosCompressao.hpp"
#include "source/language/tr_01_gerenciadorTraducao.hpp"
#include "source/utils/CaminhoPersistenteUtils.hpp"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMouseEvent>

class TestableMetodoCompressao : public MetodoCompressao {
public:
    ~TestableMetodoCompressao() override {
        delete compression_menu;
        compression_menu = nullptr;
        delete compression_method_action;
        compression_method_action = nullptr;
        delete config_menu;
        config_menu = nullptr;
    }
};

class TestCompressionMethodsConfig : public QObject {
    Q_OBJECT

private slots:
    void testCompressTypeConversions() {
        QCOMPARE(compressTypeToString(CompressType::RAR), QStringLiteral("rar"));
        QCOMPARE(compressTypeToString(CompressType::ZIP), QStringLiteral("zip"));
        QCOMPARE(compressTypeToString(CompressType::SEVEN_Z), QStringLiteral("7z"));
        QCOMPARE(compressTypeToString(CompressType::TAR), QStringLiteral("tar"));
        QCOMPARE(compressTypeToString(CompressType::WIM), QStringLiteral("wim"));

        QCOMPARE(stringToCompressType("rar"), CompressType::RAR);
        QCOMPARE(stringToCompressType("zip"), CompressType::ZIP);
        QCOMPARE(stringToCompressType("7z"), CompressType::SEVEN_Z);
        QCOMPARE(stringToCompressType("tar"), CompressType::TAR);
        QCOMPARE(stringToCompressType("wim"), CompressType::WIM);
        QCOMPARE(stringToCompressType("unknown_format"), CompressType::RAR);
    }

    void testTranslatedLegendsInBothLanguages() {
        GerenciadorTraducao gt;
        QVERIFY(gt.definir_idioma("pt_BR"));

        TranslatedLegends pt = get_translated_legends();
        QCOMPARE(pt.rar.size(), 6);
        QCOMPARE(pt.sevenz_zip.size(), 6); // 0, 1, 3, 5, 7, 9
        QCOMPARE(pt.tar.size(), 1);
        QCOMPARE(pt.wim.size(), 1);
        QCOMPARE(pt.tar_wim.size(), 1);
        QVERIFY(pt.rar.value("0").contains("Armazenar"));
        QVERIFY(pt.rar.value("5").contains("Ótimo"));
        QVERIFY(pt.sevenz_zip.value("9").contains("Ultra"));

        QVERIFY(gt.definir_idioma("en_US"));
        TranslatedLegends en = get_translated_legends();
        QCOMPARE(en.rar.size(), 6);
        QCOMPARE(en.sevenz_zip.size(), 6);
        QVERIFY(en.rar.value("0").contains("Store"));
        QVERIFY(en.rar.value("5").contains("Best"));
        QVERIFY(en.sevenz_zip.value("1").contains("Fastest"));
        QVERIFY(pt.rar.value("0") != en.rar.value("0"));

        QVERIFY(gt.definir_idioma("pt_BR"));
    }

    void testMenuPersistenteAndMethodSyncAndJsonPersistence() {
        const QString cfgPath = QDir(obter_caminho_persistente()).filePath("config.json");
        QFile::remove(cfgPath);

        GerenciadorInterface gi1;
        GerenciadorInterface gi2;

        TestableMetodoCompressao mc;
        mc.get_gerenciador_interface = [&gi1]() { return &gi1; };
        mc.get_gerenciador_interface_2 = [&gi2]() { return &gi2; };
        mc.select_compression_method();

        QCOMPARE(mc.get_config_path(), cfgPath);
        QVERIFY(mc.compression_menu != nullptr);
        QVERIFY(mc.method_actions.contains("rar"));
        QVERIFY(mc.method_actions.contains("zip"));
        QVERIFY(mc.method_actions.contains("7z"));
        QVERIFY(mc.method_actions.contains("tar"));
        QVERIFY(mc.method_actions.contains("wim"));

        // MenuPersistente nao fecha ao liberar o clique em uma action checavel sem submenu
        MenuPersistente persistMenu("Teste");
        QAction* checkAct = persistMenu.addAction("Nivel 5");
        checkAct->setCheckable(true);
        persistMenu.resize(160, 40);
        persistMenu.show();
        const QRect actRect = persistMenu.actionGeometry(checkAct);
        QMouseEvent relEv(QEvent::MouseButtonRelease, actRect.center(), persistMenu.mapToGlobal(actRect.center()),
                          Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        QCoreApplication::sendEvent(&persistMenu, &relEv);
        QVERIFY(relEv.isAccepted());
        QVERIFY(persistMenu.isVisible());
        persistMenu.hide();

        // Selecao sincronizada entre Layout 1 e Layout 2 + exclusividade mutua
        mc.set_compression_method("4", CompressType::RAR);
        mc.set_compression_method("9", CompressType::ZIP);
        mc.set_compression_method("7", CompressType::SEVEN_Z);
        mc.set_compression_method("0", CompressType::TAR);
        mc.set_compression_method("0", CompressType::WIM);

        QCOMPARE(gi1.getCompressionMethod("rar"), QStringLiteral("4"));
        QCOMPARE(gi2.getCompressionMethod("rar"), QStringLiteral("4"));
        QCOMPARE(gi1.getCompressionMethod("zip"), QStringLiteral("9"));
        QCOMPARE(gi2.getCompressionMethod("zip"), QStringLiteral("9"));
        QCOMPARE(gi1.getCompressionMethod("7z"), QStringLiteral("7"));
        QCOMPARE(gi2.getCompressionMethod("7z"), QStringLiteral("7"));
        QCOMPARE(mc.get_current_method("zip"), QStringLiteral("9"));

        // Somente uma action marcada por formato
        for (auto it = mc.method_actions["zip"].begin(); it != mc.method_actions["zip"].end(); ++it) {
            QCOMPARE(it.value()->isChecked(), it.key() == "9");
        }

        // Verifica JSON persistido
        QFile f(cfgPath);
        QVERIFY(f.open(QIODevice::ReadOnly));
        const QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
        f.close();
        QVERIFY(doc.isObject());
        const QJsonObject obj = doc.object();
        QCOMPARE(obj.value("compress_type_rar").toString(), QStringLiteral("4"));
        QCOMPARE(obj.value("compress_type_zip").toString(), QStringLiteral("9"));
        QCOMPARE(obj.value("compress_type_7z").toString(), QStringLiteral("7"));
        QCOMPARE(obj.value("compress_type_tar").toString(), QStringLiteral("0"));
        QCOMPARE(obj.value("compress_type_wim").toString(), QStringLiteral("0"));

        // Recarrega em novas instancias de GerenciadorInterface
        GerenciadorInterface giReload1;
        GerenciadorInterface giReload2;
        TestableMetodoCompressao mcReload;
        mcReload.get_gerenciador_interface = [&giReload1]() { return &giReload1; };
        mcReload.get_gerenciador_interface_2 = [&giReload2]() { return &giReload2; };
        mcReload.load_compression_method();

        QCOMPARE(giReload1.getCompressionMethod("rar"), QStringLiteral("4"));
        QCOMPARE(giReload2.getCompressionMethod("rar"), QStringLiteral("4"));
        QCOMPARE(giReload1.getCompressionMethod("zip"), QStringLiteral("9"));
        QCOMPARE(giReload2.getCompressionMethod("zip"), QStringLiteral("9"));
        QCOMPARE(giReload1.getCompressionMethod("7z"), QStringLiteral("7"));
        QCOMPARE(giReload1.getCompressionMethod("tar"), QStringLiteral("0"));
        QCOMPARE(giReload1.getCompressionMethod("wim"), QStringLiteral("0"));

        // Resiliencia contra JSON corrompido
        QVERIFY(cm_test::writeBytes(cfgPath, QByteArray("{invalid-json")));
        GerenciadorInterface giBad;
        TestableMetodoCompressao mcBad;
        mcBad.get_gerenciador_interface = [&giBad]() { return &giBad; };
        mcBad.load_compression_method();
        QVERIFY(giBad.getCompressionMethod("zip").isEmpty());
    }
};

CM_TEST_MAIN(TestCompressionMethodsConfig)
#ifndef __INTELLISENSE__
#include "test_05_compression_methods_config.moc"
#endif
