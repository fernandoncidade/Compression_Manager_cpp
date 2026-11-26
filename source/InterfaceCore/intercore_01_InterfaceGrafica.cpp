#include "intercore_01_InterfaceGrafica.hpp"
#include "GerenciadorInterfaceGrafica/ginter_01_init_menu.hpp"
#include "GerenciadorInterfaceGrafica/ginter_02_update_compression_menus.hpp"
#include "GerenciadorInterfaceGrafica/ginter_03_mudar_idioma.hpp"
#include "GerenciadorInterfaceGrafica/ginter_04_retranslateUi.hpp"
#include "GerenciadorInterfaceGrafica/ginter_05_init_ui.hpp"
#include "GerenciadorInterfaceGrafica/ginter_06_create_widget_with_layout.hpp"
#include "GerenciadorInterfaceGrafica/ginter_07_create_main_layout_1.hpp"
#include "GerenciadorInterfaceGrafica/ginter_08_create_first_quadrant.hpp"
#include "GerenciadorInterfaceGrafica/ginter_09_create_second_quadrant.hpp"
#include "GerenciadorInterfaceGrafica/ginter_10_rebuild_method_layouts.hpp"
#include "GerenciadorInterfaceGrafica/ginter_11_create_main_layout_2.hpp"
#include "GerenciadorInterfaceGrafica/ginter_12_create_third_quadrant.hpp"
#include "GerenciadorInterfaceGrafica/ginter_13_create_scroll_area.hpp"
#include "GerenciadorInterfaceGrafica/ginter_14_create_button.hpp"
#include "GerenciadorInterfaceGrafica/ginter_15_create_method_checkboxes.hpp"
#include "GerenciadorInterfaceGrafica/ginter_16_on_method_toggled.hpp"
#include "GerenciadorInterfaceGrafica/ginter_17_adjust_scroll_area.hpp"
#include "GerenciadorInterfaceGrafica/ginter_18_remove_layout_widgets.hpp"
#include "GerenciadorInterfaceGrafica/ginter_19_closeEvent.hpp"
#include "GerenciadorInterfaceGrafica/ginter_20_exibir_sobre.hpp"
#include "GerenciadorInterfaceGrafica/ginter_21_toggle_segundo_layout.hpp"
#include "GerenciadorInterfaceGrafica/ginter_22_create_scroll_area_2.hpp"
#include "GerenciadorInterfaceGrafica/ginter_23_create_first_quadrant_2.hpp"
#include "GerenciadorInterfaceGrafica/ginter_24_create_second_quadrant_2.hpp"
#include "GerenciadorInterfaceGrafica/ginter_25_create_method_checkboxes_2.hpp"
#include "GerenciadorInterfaceGrafica/ginter_26_on_method_toggled_2.hpp"
#include "GerenciadorInterfaceGrafica/ginter_27_adapt_layout_orientation.hpp"

#include <QResizeEvent>
#include <QCloseEvent>

InterfaceGrafica::InterfaceGrafica(QWidget* parent)
    : QMainWindow(parent)
{
    gerenciador_interface = std::make_unique<GerenciadorInterface>(this);
    layouts_compressao = std::make_unique<LayoutsCompressao>(
        gerenciador_interface.get(),
        [this](const QString& text, std::function<void()> cb, const QString& icon) {
            return this->create_button(text, cb, icon);
        }
    );
    compression_method_layouts = layouts_compressao->create_compression_method_layouts();

    gerenciador_interface_2 = std::make_unique<GerenciadorInterface>(this);
    layouts_compressao_2 = std::make_unique<LayoutsCompressao>(
        gerenciador_interface_2.get(),
        [this](const QString& text, std::function<void()> cb, const QString& icon) {
            return this->create_button(text, cb, icon);
        }
    );
    compression_method_layouts_2 = layouts_compressao_2->create_compression_method_layouts();

    get_gerenciador_interface = [this]() {
        return this->gerenciador_interface.get();
    };
    get_gerenciador_interface_2 = [this]() {
        return this->gerenciador_interface_2.get();
    };

    gerenciador_traducao = std::make_unique<GerenciadorTraducao>();
    connect(gerenciador_traducao.get(), &GerenciadorTraducao::idioma_alterado, this, [this]() {
        this->retranslateUi();
    });
    gerenciador_traducao->aplicar_traducao();
    this->setProperty("idioma_atual", gerenciador_traducao->obter_idioma_atual());

    init_menu();
    init_ui();
    load_compression_method();
}

InterfaceGrafica::~InterfaceGrafica() = default;

void InterfaceGrafica::init_menu() {
    ::init_menu(this);
}

void InterfaceGrafica::update_compression_menus() {
    ::update_compression_menus(this);
}

void InterfaceGrafica::mudar_idioma(const QString& codigo_idioma) {
    ::mudar_idioma(this, codigo_idioma);
}

void InterfaceGrafica::retranslateUi() {
    ::retranslateUi(this);
}

void InterfaceGrafica::init_ui() {
    ::init_ui(this);
}

QWidget* InterfaceGrafica::create_widget_with_layout(QLayout* layout) {
    return ::create_widget_with_layout(this, layout);
}

QHBoxLayout* InterfaceGrafica::create_main_layout_1() {
    return ::create_main_layout_1(this);
}

QVBoxLayout* InterfaceGrafica::create_first_quadrant() {
    return ::create_first_quadrant(this);
}

QVBoxLayout* InterfaceGrafica::create_second_quadrant() {
    return ::create_second_quadrant(this);
}

void InterfaceGrafica::rebuild_method_layouts() {
    ::rebuild_method_layouts(this);
}

QHBoxLayout* InterfaceGrafica::create_main_layout_2() {
    return ::create_main_layout_2(this);
}

QVBoxLayout* InterfaceGrafica::create_third_quadrant() {
    return ginter_create_third_quadrant(this);
}

QGroupBox* InterfaceGrafica::create_scroll_area() {
    return ginter_create_scroll_area(this);
}

QPushButton* InterfaceGrafica::create_button(const QString& text, std::function<void()> callback, const QString& icon) {
    return ginter_create_button(this, text, callback, icon);
}

void InterfaceGrafica::create_method_checkboxes(QVBoxLayout* layout) {
    ginter_create_method_checkboxes(this, layout);
}

void InterfaceGrafica::on_method_toggled() {
    ginter_on_method_toggled(this);
}

void InterfaceGrafica::adjust_scroll_area() {
    ginter_adjust_scroll_area(this);
}

void InterfaceGrafica::remove_layout_widgets(QLayout* layout) {
    ginter_remove_layout_widgets(this, layout);
}

void InterfaceGrafica::toggle_segundo_layout() {
    ginter_toggle_segundo_layout(this);
}

QGroupBox* InterfaceGrafica::create_scroll_area_2() {
    return ginter_create_scroll_area_2(this);
}

QVBoxLayout* InterfaceGrafica::create_first_quadrant_2() {
    return ginter_create_first_quadrant_2(this);
}

QVBoxLayout* InterfaceGrafica::create_second_quadrant_2() {
    return ginter_create_second_quadrant_2(this);
}

void InterfaceGrafica::create_method_checkboxes_2(QVBoxLayout* layout) {
    ginter_create_method_checkboxes_2(this, layout);
}

void InterfaceGrafica::on_method_toggled_2() {
    ginter_on_method_toggled_2(this);
}

void InterfaceGrafica::adapt_layout_orientation() {
    ginter_adapt_layout_orientation(this);
}

void InterfaceGrafica::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    adjust_scroll_area();
    adapt_layout_orientation();
}

void InterfaceGrafica::closeEvent(QCloseEvent* event) {
    ginter_closeEvent(this, event);
    QMainWindow::closeEvent(event);
}

void InterfaceGrafica::changeEvent(QEvent* event) {
    if (event->type() == QEvent::ActivationChange) {
        if (isActiveWindow()) {
            raise();
        }
    }
    QMainWindow::changeEvent(event);
}
