#ifndef GLOBAIS_H
#define GLOBAIS_H

#include "defs.h"

extern const float SPRITE_INTERNAL_CROP[4];
extern const int pontos_necessarios[MAX_NIVEIS];
extern const float SPRITE_COORDS[6][4];
extern const float MENU_DRAW_COORDS[3][4];
extern const float CUSTOM_SELECT_COORDS[4][4];
extern const float CONFIG_IMAGE_COORDS[2][4];
extern const float CONFIG_SELECT_COORDS[2][4];
extern const char* opcoes_menu[];
extern const char* opcoes_gameover_normal[];
extern const char* opcoes_gameover_com_proximo[];

extern double tempo_jogo;
extern int pontuacao_total;
extern int pontuacoes_maximas[MAX_NIVEIS];
extern int ultimas_pontuacoes[MAX_NIVEIS];
extern double tempos_por_nivel[MAX_NIVEIS];
extern int inimigos_derrotados;
extern int vezes_derrotado;
extern bool stats_mostrando;
extern int selecao_customizacao;
extern int skin_selecionada_idx;
extern int selecao_configuracao;
extern int controle_selecionado_idx;
extern int frame_animacao_inimigo;
extern int chefao_anim_frame;
extern int chefao_anim_timer;
extern PontoNivel niveis[MAX_NIVEIS];
extern int total_niveis;
extern int nivel_hover, nive_ljogando, selecao, vidas, pontos;
extern int selecao_gameover, dano_timer;
extern bool pontos_suficientes, invulneravel;
extern int life_spawn_timer;
extern int pilha_estados[10];
extern int topo_pilha;

extern ALLEGRO_BITMAP* teclas_wasd_image;
extern ALLEGRO_BITMAP* teclas_setas_image;
extern ALLEGRO_BITMAP* skin_images[4];
extern ALLEGRO_BITMAP* skin_atual_jogador;
extern ALLEGRO_BITMAP* sprites_animacao_sheets[4];
extern ALLEGRO_BITMAP* sprites_animacao_atual;
extern ALLEGRO_BITMAP* coracao_preenchido;
extern ALLEGRO_BITMAP* coracao_vazio;
extern ALLEGRO_BITMAP* inimigo_normal_1;
extern ALLEGRO_BITMAP* inimigo_normal_2;
extern ALLEGRO_BITMAP* inimigo_armadura_full;
extern ALLEGRO_BITMAP* inimigo_armadura_danificada;
extern ALLEGRO_BITMAP* inimigo_atirador_1;
extern ALLEGRO_BITMAP* inimigo_atirador_2;
extern ALLEGRO_BITMAP* inimigo_pulador_chao;
extern ALLEGRO_BITMAP* inimigo_pulador_pulo;
extern ALLEGRO_BITMAP* chefao_pequeno_1;
extern ALLEGRO_BITMAP* chefao_pequeno_2;
extern ALLEGRO_BITMAP* chefao_grande_3;
extern ALLEGRO_BITMAP* chefao_grande_4;

extern ALLEGRO_DISPLAY* display;
extern ALLEGRO_EVENT_QUEUE* queue;
extern ALLEGRO_TIMER* timer;
extern ALLEGRO_FONT* font;
extern ALLEGRO_BITMAP* mapa;
extern ALLEGRO_BITMAP* menu_image;
extern ALLEGRO_BITMAP* fundo_jogo;
extern ALLEGRO_BITMAP* customizacao_image;

#endif