#pragma once

class InterfaceGrafica;

void ginter_exibir_sobre(InterfaceGrafica* self);
inline void exibir_sobre(InterfaceGrafica* self) { ginter_exibir_sobre(self); }
void ginter_atualizar_sobre(InterfaceGrafica* self);

