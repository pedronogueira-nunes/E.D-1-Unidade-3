#include "logica.h"

float dist(float x1, float y1, float x2, float y2) {
    return sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void cria_projetil(Projetil* projeteis, float x, float y, float tx, float ty, float vel) {
    for (int i = 0; i < MAX_PROJETEIS; i++) if (!projeteis[i].ativo) {
        projeteis[i].loc.pos.x = x;
        projeteis[i].loc.pos.y = y;
        float dx = tx - x, dy = ty - y, d = sqrtf(dx * dx + dy * dy);
        if (d == 0) d = 1;
        projeteis[i].loc.vel.x = vel * dx / d;
        projeteis[i].loc.vel.y = vel * dy / d;
        projeteis[i].ativo = true;
        break;
    }
}

void resetar_inimigos(Inimigo* inimigos, int fase) {
    if (fase == MAX_NIVEIS - 1) {
        for (int i = 1; i < MAX_INIMIGOS; i++) inimigos[i].ativo = false;
        Inimigo* chefe = &inimigos[0];
        chefe->ativo = true;
        chefe->tipo = INIMIGO_CHEFAO;
        chefe->loc.pos.x = SCREEN_W / 3 - 14;
        chefe->loc.pos.y = NIVEL_H - 28 * 3;
        chefe->loc.vel.x = 0; chefe->loc.vel.y = 0;
        chefe->dir = (rand() % 2 == 0) ? 1 : -1;
        chefe->speed = 1.0f + 0.15f * fase;
        chefe->scale = 1.5f;
        chefe->scale_dir = 1;
        chefe->vida_chefo = 10;
        chefe->invulneravel = false;
        chefe->patrulha_tick = 60;
        chefe->recalc_tick = 0;
        chefe->vida_armadura = 0;
        chefe->persegue_jogador = true;
        chefe->aggro = 0; chefe->salto_duplo = 0;
        return;
    }

    int tipos_disponiveis = (fase < 4) ? fase + 1 : 4;
    int qtd_agressivos = (fase >= 1) ? 1 + rand() % (fase + 1) : 0;
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        inimigos[i].borda = i % 2;
        inimigos[i].loc.pos.x = inimigos[i].borda ? SCREEN_W - 45 : 0;
        inimigos[i].loc.pos.y = NIVEL_H - 45;
        inimigos[i].loc.vel.x = 0; inimigos[i].loc.vel.y = 0;
        inimigos[i].ativo = true;
        inimigos[i].dir = inimigos[i].borda ? -1 : 1;
        TipoInimigo tipo;
        do {
            tipo = (TipoInimigo)(rand() % tipos_disponiveis);
        } while ((tipo == INIMIGO_ARMADURA && fase < 2) || (tipo == INIMIGO_ATIRADOR && fase < 3));
        inimigos[i].tipo = tipo;
        inimigos[i].vida_armadura = (tipo == INIMIGO_ARMADURA) ? 2 : 0;
        inimigos[i].speed = (1.3f + (rand() % 70) * 0.018f) + 0.13f * fase;
        inimigos[i].aggro = 0; inimigos[i].recalc_tick = 26 + rand() % 32;
        inimigos[i].patrulha_tick = 50 + rand() % 60; inimigos[i].salto_duplo = 0;
        inimigos[i].persegue_jogador = false;
        inimigos[i].scale = 1.0f;
        inimigos[i].scale_dir = 0;
        inimigos[i].vida_chefo = 0;
        inimigos[i].invulneravel = false;
    }
    for (int i = 0, n = 0; i < MAX_INIMIGOS && n < qtd_agressivos; i++) {
        if (inimigos[i].tipo == INIMIGO_ARMADURA || inimigos[i].tipo == INIMIGO_ATIRADOR) continue;
        if (!inimigos[i].persegue_jogador && rand() % 2 == 0) {
            inimigos[i].persegue_jogador = true;
            n++;
        }
    }
}

void respawn_inimigo(Inimigo* in, int fase) {
    if (in->tipo == INIMIGO_CHEFAO) {
        in->ativo = false;
        return;
    }

    if (fase == MAX_NIVEIS - 1) {
        in->ativo = false;
        return;
    }

    int tipos_disponiveis = (fase < 4) ? fase + 1 : 4;
    in->borda = !(in->borda);
    in->loc.pos.x = in->borda ? SCREEN_W - 45 : 0;
    in->loc.pos.y = NIVEL_H - 45;
    in->loc.vel.x = 0; in->loc.vel.y = 0;
    in->ativo = true;
    in->dir = in->borda ? -1 : 1;
    TipoInimigo tipo;
    do {
        tipo = (TipoInimigo)(rand() % tipos_disponiveis);
    } while ((tipo == INIMIGO_ARMADURA && fase < 2) || (tipo == INIMIGO_ATIRADOR && fase < 3));
    in->tipo = tipo;
    in->vida_armadura = (tipo == INIMIGO_ARMADURA) ? 2 : 0;
    in->speed = (1.3f + (rand() % 70) * 0.018f) + 0.13f * fase;
    in->aggro = 0; in->recalc_tick = 26 + rand() % 32; in->patrulha_tick = 50 + rand() % 60; in->salto_duplo = 0;
    in->persegue_jogador = (fase >= 1 && (rand() % 3 == 0) && tipo != INIMIGO_ARMADURA && tipo != INIMIGO_ATIRADOR);
}

void salvar_estatisticas_em_arquivo() {
    FILE* arquivo = fopen("relatorio_jogador.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo!\n");
        return;
    }

    int pontuacao_maxima_total = 0;
    for (int i = 0; i < MAX_NIVEIS; i++) {
        if (pontuacoes_maximas[i] > pontuacao_maxima_total)
            pontuacao_maxima_total = pontuacoes_maximas[i];
    }

    fprintf(arquivo, "========================================\n");
    fprintf(arquivo, "       RELATORIO FINAL DO JOGADOR       \n");
    fprintf(arquivo, "========================================\n");
    fprintf(arquivo, "Tempo Total de Jogo: %02d:%02d\n", (int)tempo_jogo / 60, (int)tempo_jogo % 60);
    fprintf(arquivo, "Pontuacao Acumulada: %d\n", pontuacao_total);
    fprintf(arquivo, "Maior Recorde:       %d\n", pontuacao_maxima_total);
    fprintf(arquivo, "Inimigos Derrotados: %d\n", inimigos_derrotados);
    fprintf(arquivo, "Total de Mortes:     %d\n", vezes_derrotado);
    fprintf(arquivo, "\n========================================\n");
    fprintf(arquivo, "    RANKING DE NIVEIS (MAIOR PONTO)     \n");
    fprintf(arquivo, "========================================\n");

    int ordem[MAX_NIVEIS];
    for (int i = 0; i < MAX_NIVEIS; i++) ordem[i] = i;

    for (int i = 0; i < MAX_NIVEIS - 1; i++) {
        for (int j = 0; j < MAX_NIVEIS - i - 1; j++) {
            if (ultimas_pontuacoes[ordem[j]] <
                ultimas_pontuacoes[ordem[j + 1]]) {
                int temp = ordem[j];
                ordem[j] = ordem[j + 1];
                ordem[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < MAX_NIVEIS; i++) {
        int idx = ordem[i];
        if (ultimas_pontuacoes[idx] > 0) {
            fprintf(arquivo, "%d. NIVEL %d -> Pontos: %d | Tempo: %02d:%02d\n",
                i + 1, idx + 1, ultimas_pontuacoes[idx],
                (int)tempos_por_nivel[idx] / 60, (int)tempos_por_nivel[idx] % 60);
        }
    }

    fprintf(arquivo, "========================================\n");
    fclose(arquivo);
}

bool check_colisao(float x, float y) {
    return (y + PLAYER_DRAW_H >= NIVEL_H);
}

void empilhar_estado(int estado) {
    if (topo_pilha < 9) {
        pilha_estados[++topo_pilha] = estado;
    }
}

int desempilhar_estado() {
    if (topo_pilha >= 0) {
        return pilha_estados[topo_pilha--];
    }
    return MENU; 
}


