#ifndef LOGICA_H
#define LOGICA_H

#include "defs.h"
#include "globais.h"

float dist(float x1, float y1, float x2, float y2);
void cria_projetil(Projetil* projeteis, float x, float y, float tx, float ty, float vel);
void resetar_inimigos(Inimigo* inimigos, int fase);
void respawn_inimigo(Inimigo* in, int fase);
void salvar_estatisticas_em_arquivo();
bool check_colisao(float x, float y);
void empilhar_estado(int estado);
int desempilhar_estado();

#endif