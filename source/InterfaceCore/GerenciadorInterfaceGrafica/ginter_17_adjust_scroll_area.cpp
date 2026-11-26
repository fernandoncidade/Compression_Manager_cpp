#include "ginter_17_adjust_scroll_area.hpp"
#include "source/InterfaceCore/intercore_01_InterfaceGrafica.hpp"
#include "source/utils/LogManager.hpp"
#include <algorithm>

void ginter_adjust_scroll_area(InterfaceGrafica* self) {
    if (!self) return;

    int win_h = self->height();

    // 1. Ajuste do primeiro layout (scroll_area principal)
    if (self->scroll_area) {
        int num_layouts = self->current_layouts.size();

        if (num_layouts <= 1) {
            // Quando 0 ou apenas 1 modo estiver ativo:
            // O modo único ajusta-se dinamicamente em tempo de execução às dimensões da janela;
            // A barra de rolagem vertical permanece desativada
            self->scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            self->scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
            self->scroll_area->setMinimumHeight(num_layouts == 0 ? 120 : 130);
        } else {
            // A partir do segundo modo de compressão/extração que for ativado:
            // A barra de rolagem vertical é ativada dinamicamente em tempo de execução
            self->scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
            self->scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
            self->scroll_area->setMinimumHeight(140);
        }

        if (self->main_layout_1_widget) {
            // Sistema dinâmico e inteligente para redimensionamento do quadrante superior
            // Mantém os limites respeitando os widgets (sem vazamento ou estouro)
            if (num_layouts == 0) {
                self->main_layout_1_widget->setMaximumHeight(304);
            } else if (num_layouts == 1) {
                // Em dimensões iniciais (<= 550px), limite a 260px para dar folga perfeita ao modo ativo
                // Conforme o usuário redimensiona até a maximização, expande suavemente até 304px
                int max_h = (win_h > 600) ? 304 : std::clamp(win_h - 260, 152, 260);
                self->main_layout_1_widget->setMaximumHeight(max_h);
            } else {
                // A partir de 2 modos, prioriza o espaço da scroll_area para exibir os modos
                int max_h = (win_h > 700) ? 260 : 210;
                self->main_layout_1_widget->setMaximumHeight(max_h);
            }
        }
    }

    // 2. Ajuste do segundo layout (scroll_area_2), quando ativo
    bool is_segundo_ativo = (self->segundo_layout_action && 
                             self->segundo_layout_action->isChecked() && 
                             self->segundo_layout_widget != nullptr);
    if (is_segundo_ativo && self->scroll_area_2) {
        int num_layouts_2 = self->current_layouts_2.size();

        if (num_layouts_2 <= 1) {
            self->scroll_area_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            self->scroll_area_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
            self->scroll_area_2->setMinimumHeight(num_layouts_2 == 0 ? 120 : 130);
        } else {
            self->scroll_area_2->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
            self->scroll_area_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
            self->scroll_area_2->setMinimumHeight(140);
        }

        if (self->main_layout_1_widget_2) {
            if (num_layouts_2 == 0) {
                self->main_layout_1_widget_2->setMaximumHeight(304);
            } else if (num_layouts_2 == 1) {
                int max_h = (win_h > 600) ? 304 : std::clamp(win_h - 260, 152, 260);
                self->main_layout_1_widget_2->setMaximumHeight(max_h);
            } else {
                int max_h = (win_h > 700) ? 260 : 210;
                self->main_layout_1_widget_2->setMaximumHeight(max_h);
            }
        }
    }
}
