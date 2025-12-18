#ifndef DEFS_H
#define DEFS_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>


#define SCREEN_W 1350
#define SCREEN_H 800
#define MAPA_TAM 700
#define MAPA_OFFSET_X ((SCREEN_W - MAPA_TAM)/2)
#define MAPA_OFFSET_Y ((SCREEN_H - MAPA_TAM)/2)
#define NIVEL_W 600
#define NIVEL_H 650 
#define VIDAS_INICIAIS 5
#define MAX_INIMIGOS 6
#define MAX_PROJETEIS 20
#define MAX_NIVEIS 6
#define MAX_LIFES 3 

#define SPRITE_W 203 
#define SPRITE_H 205 
#define PLAYER_DRAW_W 30 
#define PLAYER_DRAW_H 60 


typedef enum { MENU, CUSTOM, MAPA, JOGO, GAMEOVER, CONFIG, SAIR } EstadoJogo;
typedef enum { INIMIGO_NORMAL = 0, INIMIGO_PULADOR = 1, INIMIGO_ARMADURA = 2, INIMIGO_ATIRADOR = 3, INIMIGO_CHEFAO = 4 } TipoInimigo;


typedef struct {
    float x, y;
} Vector2;

typedef struct {
    Vector2 pos;
    Vector2 vel;
} Fisica;

typedef struct {
    float x, y;
    bool desbloqueado;
} PontoNivel;

typedef struct {
    Fisica loc;
    bool no_chao;
    int dir_x;
    int frame_atual;
    float anim_timer;
} Personagem;

typedef struct {
    Fisica loc;
    int dir;
    int borda;
    int patrulha_tick, recalc_tick, aggro, salto_duplo;
    float speed;
    bool ativo;
    TipoInimigo tipo;
    int vida_armadura;
    bool persegue_jogador;
    float scale;
    int scale_dir;
    int vida_chefo;
    bool invulneravel;
} Inimigo;

typedef struct {
    Fisica loc;
    bool ativo;
} Projetil;

typedef struct {
    Vector2 pos;
    bool ativo;
} VidaExtra;

typedef struct {
    char* nome_sessao;
    int id_sessao;
} SessaoJogo;


#endif