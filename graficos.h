#ifndef GRAFICOS_H
#define GRAFICOS_H

#include "defs.h"
#include "globais.h"

void desenhar_stats();
void desenhar_menu();
void desenhar_configuracao();
void desenhar_customizacao();
void desenhar_gameover(int pontos, bool pontos_suf, int selecao, int nivel);
void desenhar_mapa_niveis();
void desenhar_jogo(Personagem* p, int vidas_atual, Inimigo* inimigos, VidaExtra* vidas_extras, Projetil* projeteis, int pontos, bool pontos_suf, int nivel);

#endif