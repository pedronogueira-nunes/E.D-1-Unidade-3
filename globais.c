#include "globais.h"


const float SPRITE_INTERNAL_CROP[4] = { 26.0f, 20.0f, 130.0f, 170.0f };
const int pontos_necessarios[MAX_NIVEIS] = { 100, 100, 100, 100, 500, 1 };
const float SPRITE_COORDS[6][4] = {
    { 0 * SPRITE_W, 0 * SPRITE_H, SPRITE_W, SPRITE_H },
    { 1 * SPRITE_W, 0 * SPRITE_H, SPRITE_W, SPRITE_H },
    { 2 * SPRITE_W, 0 * SPRITE_H, SPRITE_W, SPRITE_H },
    { 0 * SPRITE_W, 1 * SPRITE_H, SPRITE_W, SPRITE_H },
    { 1 * SPRITE_W, 1 * SPRITE_H, SPRITE_W, SPRITE_H },
    { 2 * SPRITE_W, 1 * SPRITE_H, SPRITE_W, SPRITE_H }
};
const float MENU_DRAW_COORDS[3][4] = {
    { 200.0f, 421.0f, 525.0f, 615.0f },
    { 500.0f, 807.0f, 527.0f, 617.0f },
    { 857.0f, 1179.0f, 527.0f, 617.0f }
};
const float CUSTOM_SELECT_COORDS[4][4] = {
    { 157.0f, 400.0f, 587.0f, 645.0f },
    { 432.0f, 673.0f, 587.0f, 645.0f },
    { 704.0f, 945.0f, 587.0f, 645.0f },
    { 977.0f, 1218.0f, 587.0f, 645.0f }
};
const float CONFIG_IMAGE_COORDS[2][4] = {
    { 250.0f, 200.0f, 300.0f, 300.0f },
    { 800.0f, 200.0f, 300.0f, 300.0f }
};
const float CONFIG_SELECT_COORDS[2][4] = {
    { 250.0f, 550.0f, 520.0f, 585.0f },
    { 800.0f, 1100.0f, 520.0f, 585.0f }
};

const char* opcoes_menu[] = { "PLAY", "CUSTOM", "CONFIG" };
const char* opcoes_gameover_normal[] = { "Reiniciar", "Voltar ao mapa" };
const char* opcoes_gameover_com_proximo[] = { "Reiniciar", "Proximo nivel", "Voltar ao mapa" };


double tempo_jogo = 0;
int pontuacao_total = 0;
int pontuacoes_maximas[MAX_NIVEIS] = { 0 };
int ultimas_pontuacoes[MAX_NIVEIS] = { 0 };
double tempos_por_nivel[MAX_NIVEIS] = { 0 };
int inimigos_derrotados = 0;
int vezes_derrotado = 0;
bool stats_mostrando = false;
int selecao_customizacao = -1;
int skin_selecionada_idx = 0;
int selecao_configuracao = -1;
int controle_selecionado_idx = 0;
int frame_animacao_inimigo = 0;
int chefao_anim_frame = 0;
int chefao_anim_timer = 0;
int total_niveis = MAX_NIVEIS;
int nivel_hover = -1, nive_ljogando = 0, selecao = -1, vidas = VIDAS_INICIAIS, pontos = 0;
int selecao_gameover = 0, dano_timer = 0;
bool pontos_suficientes = false, invulneravel = false;
int life_spawn_timer = 0;
int pilha_estados[10];
int topo_pilha = -1;

PontoNivel niveis[MAX_NIVEIS] = {
    {MAPA_OFFSET_X + 320, MAPA_OFFSET_Y + 310, true},
    {MAPA_OFFSET_X + 340, MAPA_OFFSET_Y + 370, false},
    {MAPA_OFFSET_X + 390, MAPA_OFFSET_Y + 410, false},
    {MAPA_OFFSET_X + 340, MAPA_OFFSET_Y + 430, false},
    {MAPA_OFFSET_X + 310, MAPA_OFFSET_Y + 416, false},
    {MAPA_OFFSET_X + 280, MAPA_OFFSET_Y + 440, false}
};


ALLEGRO_BITMAP* teclas_wasd_image = NULL;
ALLEGRO_BITMAP* teclas_setas_image = NULL;
ALLEGRO_BITMAP* skin_images[4] = { NULL };
ALLEGRO_BITMAP* skin_atual_jogador = NULL;
ALLEGRO_BITMAP* sprites_animacao_sheets[4] = { NULL };
ALLEGRO_BITMAP* sprites_animacao_atual = NULL;
ALLEGRO_BITMAP* coracao_preenchido = NULL;
ALLEGRO_BITMAP* coracao_vazio = NULL;
ALLEGRO_BITMAP* inimigo_normal_1 = NULL;
ALLEGRO_BITMAP* inimigo_normal_2 = NULL;
ALLEGRO_BITMAP* inimigo_armadura_full = NULL;
ALLEGRO_BITMAP* inimigo_armadura_danificada = NULL;
ALLEGRO_BITMAP* inimigo_atirador_1 = NULL;
ALLEGRO_BITMAP* inimigo_atirador_2 = NULL;
ALLEGRO_BITMAP* inimigo_pulador_chao = NULL;
ALLEGRO_BITMAP* inimigo_pulador_pulo = NULL;
ALLEGRO_BITMAP* chefao_pequeno_1 = NULL;
ALLEGRO_BITMAP* chefao_pequeno_2 = NULL;
ALLEGRO_BITMAP* chefao_grande_3 = NULL;
ALLEGRO_BITMAP* chefao_grande_4 = NULL;

ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE* queue = NULL;
ALLEGRO_TIMER* timer = NULL;
ALLEGRO_FONT* font = NULL;
ALLEGRO_BITMAP* mapa = NULL;
ALLEGRO_BITMAP* menu_image = NULL;
ALLEGRO_BITMAP* fundo_jogo = NULL;
ALLEGRO_BITMAP* customizacao_image = NULL;