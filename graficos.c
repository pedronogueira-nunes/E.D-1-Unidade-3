#include "graficos.h"


void desenhar_stats() {
    al_draw_filled_rounded_rectangle(SCREEN_W / 2 - 220, SCREEN_H / 2 - 180, SCREEN_W / 2 + 220, SCREEN_H / 2 + 180, 26, 26, al_map_rgba(40, 40, 60, 0.7));
    char info[128];
    sprintf(info, "Tempo de jogo:   %02d:%02d", (int)tempo_jogo / 60, (int)tempo_jogo % 60);
    al_draw_text(font, al_map_rgb(255, 255, 180), SCREEN_W / 2, SCREEN_H / 2 - 120, ALLEGRO_ALIGN_CENTER, info);
    sprintf(info, "Pontuacao total:  %d", pontuacao_total);
    al_draw_text(font, al_map_rgb(120, 255, 180), SCREEN_W / 2, SCREEN_H / 2 - 72, ALLEGRO_ALIGN_CENTER, info);

    int pontuacao_maxima_total = 0;
    for (int i = 0; i < total_niveis; i++) {
        if (pontuacoes_maximas[i] > pontuacao_maxima_total)
            pontuacao_maxima_total = pontuacoes_maximas[i];
    }

    sprintf(info, "Pontuacao maxima: %d", pontuacao_maxima_total);
    al_draw_text(font, al_map_rgb(120, 255, 120), SCREEN_W / 2, SCREEN_H / 2 - 24, ALLEGRO_ALIGN_CENTER, info);
    sprintf(info, "Inimigos derrotados: %d", inimigos_derrotados);
    al_draw_text(font, al_map_rgb(220, 120, 120), SCREEN_W / 2, SCREEN_H / 2 + 24, ALLEGRO_ALIGN_CENTER, info);
    sprintf(info, "Numero de derrotas:  %d", vezes_derrotado);
    al_draw_text(font, al_map_rgb(255, 180, 180), SCREEN_W / 2, SCREEN_H / 2 + 72, ALLEGRO_ALIGN_CENTER, info);
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 + 130, ALLEGRO_ALIGN_CENTER, "ESC para fechar");
}

void desenhar_menu() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    if (menu_image) {
        al_draw_scaled_bitmap(menu_image, 0, 0, al_get_bitmap_width(menu_image), al_get_bitmap_height(menu_image),
            0, 0, SCREEN_W, SCREEN_H, 0);
        for (int i = 0; i < 3; i++) {
            float x1 = MENU_DRAW_COORDS[i][0] * SCREEN_W / 1350.0f;
            float x2 = MENU_DRAW_COORDS[i][1] * SCREEN_W / 1350.0f;
            float y1 = MENU_DRAW_COORDS[i][2] * SCREEN_H / 800.0f;
            float y2 = MENU_DRAW_COORDS[i][3] * SCREEN_H / 800.0f;

            if (i == selecao) {
                al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgba(255, 0, 0, 100));
            }
        }
    }
    else {
        al_draw_text(font, al_map_rgb(255, 180, 180),
            SCREEN_W / 2, 160, ALLEGRO_ALIGN_CENTER,
            "THE ABYSS (Sem Imagem)");
        int box_w = 320;
        int box_h = 60;
        int start_y = 260;
        int spacing = 80;
        for (int i = 0; i < 3; i++) {
            ALLEGRO_COLOR corf = (i == selecao) ?
                al_map_rgba_f(1, 0, 0, 0.7f) : al_map_rgba_f(0, 0, 0, 0);
            ALLEGRO_COLOR cort = (i == selecao) ?
                al_map_rgb(255, 255, 255) : al_map_rgb(180, 180, 180);

            int center_y = start_y + i * spacing;
            int x1 = SCREEN_W / 2 - box_w / 2;
            int x2 = SCREEN_W / 2 + box_w / 2;
            int y1 = center_y - box_h / 2;
            int y2 = center_y + box_h / 2;
            al_draw_filled_rounded_rectangle(x1, y1, x2, y2, 15, 15, corf);
            al_draw_text(font, cort, SCREEN_W / 2, center_y - 14, ALLEGRO_ALIGN_CENTER, opcoes_menu[i]);
        }
    }
}

void desenhar_configuracao() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, 80, ALLEGRO_ALIGN_CENTER, "CHOOSE THE CONTROL SCHEME.");
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 40, ALLEGRO_ALIGN_CENTER, "ESC para voltar ao Menu");
    for (int i = 0; i < 2; i++) {
        ALLEGRO_BITMAP* img = (i == 0) ?
            teclas_wasd_image : teclas_setas_image;
        const char* label = (i == 0) ? "WASD" : "SETAS";
        float img_x = CONFIG_IMAGE_COORDS[i][0] * SCREEN_W / 1350.0f;
        float img_y = CONFIG_IMAGE_COORDS[i][1] * SCREEN_H / 800.0f;
        float img_w = CONFIG_IMAGE_COORDS[i][2] * SCREEN_W / 1350.0f;
        float img_h = CONFIG_IMAGE_COORDS[i][3] * SCREEN_H / 800.0f;
        float btn_x1 = CONFIG_SELECT_COORDS[i][0] * SCREEN_W / 1350.0f;
        float btn_x2 = CONFIG_SELECT_COORDS[i][1] * SCREEN_W / 1350.0f;
        float btn_y1 = CONFIG_SELECT_COORDS[i][2] * SCREEN_H / 800.0f;
        float btn_y2 = CONFIG_SELECT_COORDS[i][3] * SCREEN_H / 800.0f;
        if (img) {
            al_draw_scaled_bitmap(img,
                0, 0, al_get_bitmap_width(img), al_get_bitmap_height(img),
                img_x, img_y, img_w, img_h, 0);
        }
        else {
            al_draw_filled_rectangle(img_x, img_y, img_x + img_w, img_y + img_h, al_map_rgb(50, 50, 50));
            al_draw_text(font, al_map_rgb(255, 255, 255), img_x + img_w / 2, img_y + img_h / 2 - 12, ALLEGRO_ALIGN_CENTER, label);
        }

        if (i == controle_selecionado_idx) {
            float border_size = 5.0f;
            al_draw_rounded_rectangle(img_x - border_size, img_y - border_size, img_x + img_w + border_size, img_y + img_h + border_size, 10, 10, al_map_rgb(0, 255, 0), border_size);
        }

        if (i == selecao_configuracao) {
            al_draw_filled_rectangle(btn_x1, btn_y1, btn_x2, btn_y2, al_map_rgba(255, 0, 0, 120));
        }

        al_draw_text(font, al_map_rgb(255, 255, 255), (btn_x1 + btn_x2) / 2, btn_y1 + 5, ALLEGRO_ALIGN_CENTER, "SELECT");
    }
}

void desenhar_customizacao() {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    if (customizacao_image) {
        al_draw_scaled_bitmap(customizacao_image, 0, 0, al_get_bitmap_width(customizacao_image), al_get_bitmap_height(customizacao_image),
            0, 0, SCREEN_W, SCREEN_H, 0);
        for (int i = 0; i < 4; i++) {
            if (!skin_images[i]) continue;
            float x1_caixa = CUSTOM_SELECT_COORDS[i][0] * SCREEN_W / 1350.0f;
            float x2_caixa = CUSTOM_SELECT_COORDS[i][1] * SCREEN_W / 1350.0f;
            float y1_caixa = CUSTOM_SELECT_COORDS[i][2] * SCREEN_H / 800.0f;
            float draw_w = 120;
            float draw_h = 230;
            float boneco_x = x1_caixa + (x2_caixa - x1_caixa) / 2.0f - draw_w / 2.0f;
            float boneco_y = y1_caixa - 267.0f;
            al_draw_scaled_bitmap(skin_images[i],
                0, 0, al_get_bitmap_width(skin_images[i]), al_get_bitmap_height(skin_images[i]),
                boneco_x, boneco_y,
                draw_w, draw_h, 0);
            if (i == skin_selecionada_idx) {
                float border_size = 5.0f;
                float border_x1 = boneco_x - border_size;
                float border_y1 = boneco_y - border_size;
                float border_x2 = boneco_x + draw_w + border_size;
                float border_y2 = boneco_y + draw_h + border_size;

                al_draw_rounded_rectangle(border_x1, border_y1, border_x2, border_y2, 10, 10, al_map_rgb(255, 255, 0), border_size);
            }

            if (i == selecao_customizacao) {
                float x1 = CUSTOM_SELECT_COORDS[i][0] * SCREEN_W / 1350.0f;
                float x2 = CUSTOM_SELECT_COORDS[i][1] * SCREEN_W / 1350.0f;
                float y1 = CUSTOM_SELECT_COORDS[i][2] * SCREEN_H / 800.0f;
                float y2 = CUSTOM_SELECT_COORDS[i][3] * SCREEN_H / 800.0f;

                al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgba(255, 0, 0, 120));
            }
        }
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 40, ALLEGRO_ALIGN_CENTER, "ESC para voltar ao Menu");
    }
    else {
        al_draw_text(font, al_map_rgb(255, 180, 180), SCREEN_W / 2, SCREEN_H / 2, ALLEGRO_ALIGN_CENTER, "TELA DE CUSTOMIZACAO (Imagem nao carregada)");
        al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H - 40, ALLEGRO_ALIGN_CENTER, "ESC para voltar ao Menu");
    }
}

void desenhar_gameover(int pontos, bool pontos_suf, int selecao, int nivel) {
    al_clear_to_color(al_map_rgb(30, 20, 40));
    if (!(nivel == MAX_NIVEIS - 1 && pontos_suf)) {
        al_draw_text(font, al_map_rgb(255, 30, 30), SCREEN_W / 2, 140, ALLEGRO_ALIGN_CENTER, "GAME OVER");
    }

    char txt[64];
    ALLEGRO_COLOR corpts = pontos_suf ? al_map_rgb(40, 255, 40) : al_map_rgb(255, 255, 140);
    if (nivel == MAX_NIVEIS - 1 && pontos_suf) {
        al_draw_text(font, al_map_rgb(40, 255, 40), SCREEN_W / 2, 175, ALLEGRO_ALIGN_CENTER, "CHEFAO DERROTADO!");
    }

    sprintf(txt, "Pontuacao: %04d / %d", pontos, pontos_necessarios[nivel]);
    al_draw_text(font, corpts, SCREEN_W / 2, 210, ALLEGRO_ALIGN_CENTER, txt);
    if (pontos_suf && nivel < MAX_NIVEIS - 1) al_draw_text(font, al_map_rgb(30, 255, 60), SCREEN_W / 2, 250, ALLEGRO_ALIGN_CENTER, "LEVEL UP!");
    bool proximo_liberado = (nive_ljogando < total_niveis - 1) && (niveis[nive_ljogando + 1].desbloqueado);
    const char** opcoes;
    int num_opcoes;
    if (proximo_liberado) {
        opcoes = opcoes_gameover_com_proximo;
        num_opcoes = 3;
    }
    else {
        opcoes = opcoes_gameover_normal;
        num_opcoes = 2;
    }
    for (int i = 0; i < num_opcoes; i++) {
        ALLEGRO_COLOR corf = (i == selecao) ?
            al_map_rgba_f(1, 0, 0, 0.7f) : al_map_rgba_f(0, 0, 0, 0);
        ALLEGRO_COLOR cort = (i == selecao) ?
            al_map_rgb(255, 255, 255) : al_map_rgb(180, 180, 180);
        int y = 320 + i * 60;
        al_draw_filled_rounded_rectangle(SCREEN_W / 2 - 220, y - 15, SCREEN_W / 2 + 220, y + 45, 10, 10, corf);
        al_draw_text(font, cort, SCREEN_W / 2, y, ALLEGRO_ALIGN_CENTER, opcoes[i]);
    }
}

void desenhar_mapa_niveis() {
    al_clear_to_color(al_map_rgb(13, 16, 24));
    int botao_w = 160, botao_h = 38;
    int botao_x = SCREEN_W - botao_w - 20;
    int botao_y = MAPA_OFFSET_Y - botao_h - 10;
    al_draw_filled_rounded_rectangle(botao_x, botao_y, botao_x + botao_w, botao_y + botao_h, 12, 12, al_map_rgb(80, 80, 80));
    al_draw_text(font, al_map_rgb(250, 250, 150), botao_x + botao_w / 2, botao_y + 8, ALLEGRO_ALIGN_CENTER, "Status");
    if (mapa) al_draw_scaled_bitmap(mapa, 0, 0, al_get_bitmap_width(mapa), al_get_bitmap_height(mapa),
        MAPA_OFFSET_X, MAPA_OFFSET_Y, MAPA_TAM, MAPA_TAM, 0);
    else al_draw_filled_rectangle(MAPA_OFFSET_X, MAPA_OFFSET_Y, MAPA_OFFSET_X + MAPA_TAM, MAPA_OFFSET_Y + MAPA_TAM, al_map_rgb(40, 80, 120));
    for (int i = 0; i < total_niveis; i++) {
        float raio = (nivel_hover == i && niveis[i].desbloqueado) ?
            10 : 5;
        ALLEGRO_COLOR cor = al_map_rgb(200, 30, 30);
        al_draw_filled_circle(niveis[i].x, niveis[i].y, raio, cor);
        if (niveis[i].desbloqueado)
            al_draw_circle(niveis[i].x, niveis[i].y, raio + 2, al_map_rgb(255, 255, 170), 3);
        else
            al_draw_filled_circle(niveis[i].x, niveis[i].y, raio - 1, al_map_rgb(60, 15, 15));
    }
    al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, MAPA_OFFSET_Y + MAPA_TAM + 10, ALLEGRO_ALIGN_CENTER, "ESC para voltar");
}

void desenhar_jogo(Personagem* p, int vidas_atual, Inimigo* inimigos, VidaExtra* vidas_extras, Projetil* projeteis, int pontos, bool pontos_suf, int nivel) {

    if (fundo_jogo) {
        al_draw_scaled_bitmap(fundo_jogo,
            0, 0, al_get_bitmap_width(fundo_jogo), al_get_bitmap_height(fundo_jogo),
            0, 0, SCREEN_W, SCREEN_H, 0);
    }
    else {
        al_clear_to_color(al_map_rgb(25, 35, 50));
    }

    char nome_nivel[32];
    sprintf(nome_nivel, (nivel == MAX_NIVEIS - 1) ? "FINAL BOSS! (Nivel %d)" : "Nivel %d", nivel + 1);
    al_draw_text(font, al_map_rgb(180, 255, 255), SCREEN_W / 2, 10, ALLEGRO_ALIGN_CENTER, nome_nivel);

    char txt[64]; sprintf(txt, "Pontos: %04d / %d", pontos, pontos_necessarios[nivel]);
    ALLEGRO_COLOR corpts = pontos_suf ? al_map_rgb(40, 255, 40) : al_map_rgb(255, 255, 100);
    al_draw_text(font, corpts, 50, 10, 0, txt);
    if (pontos_suf && nivel < MAX_NIVEIS - 1) al_draw_text(font, al_map_rgb(30, 255, 60), 50, 45, 0, "LEVEL UP!");
    int heart_w = 40;
    int heart_h = 40;
    int start_x = 30;
    int start_y = SCREEN_H - 60;
    int spacing = 50;

    for (int i = 0; i < VIDAS_INICIAIS; i++) {
        int x = start_x + i * spacing;
        ALLEGRO_BITMAP* heart_image = (i < vidas_atual) ? coracao_preenchido : coracao_vazio;
        if (heart_image) {
            al_draw_scaled_bitmap(heart_image,
                0, 0, al_get_bitmap_width(heart_image), al_get_bitmap_height(heart_image),
                x, start_y, heart_w, heart_h, 0);
        }
        else {
            ALLEGRO_COLOR cor = (i < vidas_atual) ?
                al_map_rgb(255, 0, 0) : al_map_rgb(80, 0, 0);
            al_draw_filled_triangle(x + 10, start_y + 20, x + 20, start_y, x + 30, start_y + 20, cor);
            al_draw_filled_circle(x + 7, start_y + 15, 7, cor);
            al_draw_filled_circle(x + 33, start_y + 15, 7, cor);
        }
    }

    if (invulneravel && (dano_timer / 3) % 2 == 0) {
        al_draw_filled_rectangle(p->loc.pos.x, p->loc.pos.y, p->loc.pos.x + PLAYER_DRAW_W, p->loc.pos.y + PLAYER_DRAW_H, al_map_rgb(180, 180, 180));
    }
    else {
        ALLEGRO_BITMAP* sprite_origem = skin_atual_jogador;
        ALLEGRO_BITMAP* sprite_anim = sprites_animacao_atual;
        float sx = 0, sy = 0, sw = 0, sh = 0;
        int flags = 0;

        float crop_x_offset = SPRITE_INTERNAL_CROP[0];
        float crop_y_offset = SPRITE_INTERNAL_CROP[1];
        float crop_w = SPRITE_INTERNAL_CROP[2];
        float crop_h = SPRITE_INTERNAL_CROP[3];
        if (!p->no_chao) {
            if (sprite_anim) {
                sprite_origem = sprite_anim;
                sx = SPRITE_COORDS[0][0] + crop_x_offset;
                sy = SPRITE_COORDS[0][1] + crop_y_offset;
                sw = crop_w;
                sh = crop_h;
                if (p->dir_x == -1) flags = ALLEGRO_FLIP_HORIZONTAL;
            }
            else {
                sprite_origem = skin_atual_jogador;
            }
        }
        else if (p->loc.vel.x != 0) {
            if (sprite_anim) {
                sprite_origem = sprite_anim;
                int frame_idx = 1 + (p->frame_atual % 5);
                sx = SPRITE_COORDS[frame_idx][0] + crop_x_offset;
                sy = SPRITE_COORDS[frame_idx][1] + crop_y_offset;
                sw = crop_w;
                sh = crop_h;

                if (p->loc.vel.x < 0) flags = ALLEGRO_FLIP_HORIZONTAL;
            }
            else {
                sprite_origem = skin_atual_jogador;
            }
        }
        else {
            sprite_origem = skin_atual_jogador;
        }

        if (sprite_origem) {
            if (sprite_origem == sprite_anim) {
                al_draw_scaled_bitmap(sprite_origem,
                    sx, sy, sw, sh,
                    p->loc.pos.x, p->loc.pos.y, PLAYER_DRAW_W, PLAYER_DRAW_H, flags);
            }
            else {
                al_draw_scaled_bitmap(sprite_origem,
                    0, 0, al_get_bitmap_width(sprite_origem), al_get_bitmap_height(sprite_origem),
                    p->loc.pos.x, p->loc.pos.y, PLAYER_DRAW_W, PLAYER_DRAW_H, 0);
            }
        }
        else {
            al_draw_filled_rectangle(p->loc.pos.x, p->loc.pos.y, p->loc.pos.x + PLAYER_DRAW_W, p->loc.pos.y + PLAYER_DRAW_H, al_map_rgb(255, 220, 0));
        }
    }


    for (int i = 0; i < MAX_INIMIGOS; i++) {
        if (!inimigos[i].ativo) continue;
        Inimigo* in = &inimigos[i];

        ALLEGRO_COLOR col;
        float w = 45;
        float h = 45;
        int flags = 0;
        if (in->dir < 0) flags = ALLEGRO_FLIP_HORIZONTAL;
        if (in->tipo == INIMIGO_CHEFAO) {
            w = 28 * in->scale;
            h = 28 * in->scale;
            ALLEGRO_BITMAP* sprite_chefao = NULL;

            if (in->invulneravel) {
                sprite_chefao = (chefao_anim_frame == 0) ?
                    chefao_grande_3 : chefao_grande_4;
            }
            else {
                sprite_chefao = (chefao_anim_frame == 0) ?
                    chefao_pequeno_1 : chefao_pequeno_2;
            }

            if (in->vida_chefo > 0) {
                float barra_w = 150;
                float vida_pct = (float)in->vida_chefo / 10.0f;
                al_draw_filled_rectangle(SCREEN_W / 2 - barra_w / 2, 70, SCREEN_W / 2 + barra_w / 2, 85, al_map_rgb(40, 40, 40));
                al_draw_filled_rectangle(SCREEN_W / 2 - barra_w / 2, 70, SCREEN_W / 2 - barra_w / 2 + barra_w * vida_pct, 85, al_map_rgb(255, 0, 0));
                al_draw_text(font, al_map_rgb(255, 255, 255), SCREEN_W / 2, 90, ALLEGRO_ALIGN_CENTER, in->invulneravel ? "INVENCIBLE" : "VULNERABLE");
            }

            if (sprite_chefao) {
                al_draw_scaled_bitmap(sprite_chefao,
                    0, 0, al_get_bitmap_width(sprite_chefao), al_get_bitmap_height(sprite_chefao),
                    in->loc.pos.x, in->loc.pos.y, w, h, flags);
            }
            else {
                col = in->invulneravel ?
                    al_map_rgb(180, 50, 50) : al_map_rgb(255, 100, 100);
                al_draw_filled_rectangle(in->loc.pos.x, in->loc.pos.y, in->loc.pos.x + w, in->loc.pos.y + h, col);
                al_draw_filled_circle(in->loc.pos.x + w / 4, in->loc.pos.y + h / 4, w / 10, al_map_rgb(0, 0, 0));
                al_draw_filled_circle(in->loc.pos.x + 3 * w / 4, in->loc.pos.y + h / 4, w / 10, al_map_rgb(0, 0, 0));
            }

        }
        else if (in->tipo == INIMIGO_NORMAL) {
            ALLEGRO_BITMAP* sprite_normal = (frame_animacao_inimigo == 0) ?
                inimigo_normal_1 : inimigo_normal_2;

            if (sprite_normal) {
                al_draw_scaled_bitmap(sprite_normal,
                    0, 0, al_get_bitmap_width(sprite_normal), al_get_bitmap_height(sprite_normal),
                    in->loc.pos.x, in->loc.pos.y, w, h, flags);
            }
            else {
                al_draw_filled_rectangle(in->loc.pos.x, in->loc.pos.y, in->loc.pos.x + w, in->loc.pos.y + h, al_map_rgb(200, 30, 60));
                al_draw_filled_rectangle(in->loc.pos.x + 10, in->loc.pos.y + 41, in->loc.pos.x + 35, in->loc.pos.y + 45, al_map_rgb(0, 0, 0));
            }

        }
        else if (in->tipo == INIMIGO_ARMADURA) {
            ALLEGRO_BITMAP* sprite_armadura = NULL;
            if (in->vida_armadura == 2) {
                sprite_armadura = inimigo_armadura_full;
            }
            else {
                sprite_armadura = inimigo_armadura_danificada;
            }

            if (sprite_armadura) {
                al_draw_scaled_bitmap(sprite_armadura,
                    0, 0, al_get_bitmap_width(sprite_armadura), al_get_bitmap_height(sprite_armadura),
                    in->loc.pos.x, in->loc.pos.y, w, h, flags);
            }
            else {
                col = (in->vida_armadura == 2) ?
                    al_map_rgb(120, 110, 110) : al_map_rgb(230, 170, 0);
                al_draw_filled_rectangle(in->loc.pos.x, in->loc.pos.y, in->loc.pos.x + w, in->loc.pos.y + h, col);
                al_draw_filled_rectangle(in->loc.pos.x + 10, in->loc.pos.y + 41, in->loc.pos.x + 35, in->loc.pos.y + 45, al_map_rgb(0, 0, 0));
            }
        }
        else if (in->tipo == INIMIGO_ATIRADOR) {
            ALLEGRO_BITMAP* sprite_atirador = (frame_animacao_inimigo == 0) ?
                inimigo_atirador_1 : inimigo_atirador_2;

            if (sprite_atirador) {
                al_draw_scaled_bitmap(sprite_atirador,
                    0, 0, al_get_bitmap_width(sprite_atirador), al_get_bitmap_height(sprite_atirador),
                    in->loc.pos.x, in->loc.pos.y, w, h, flags);
            }
            else {
                col = al_map_rgb(0, 180, 100);
                al_draw_filled_rectangle(in->loc.pos.x, in->loc.pos.y, in->loc.pos.x + w, in->loc.pos.y + h, col);
                al_draw_filled_rectangle(in->loc.pos.x + 10, in->loc.pos.y + 41, in->loc.pos.x + 35, in->loc.pos.y + 45, al_map_rgb(0, 0, 0));
            }
        }
        else if (in->tipo == INIMIGO_PULADOR) {
            ALLEGRO_BITMAP* sprite_pulador = NULL;
            if (in->loc.vel.y == 0) {
                sprite_pulador = inimigo_pulador_chao;
            }
            else {
                sprite_pulador = inimigo_pulador_pulo;
            }

            if (sprite_pulador) {
                al_draw_scaled_bitmap(sprite_pulador,
                    0, 0, al_get_bitmap_width(sprite_pulador), al_get_bitmap_height(sprite_pulador),
                    in->loc.pos.x, in->loc.pos.y, w, h, flags);
            }
            else {
                col = al_map_rgb(80, 50, 220);
                al_draw_filled_rectangle(in->loc.pos.x, in->loc.pos.y, in->loc.pos.x + w, in->loc.pos.y + h, col);
                al_draw_filled_rectangle(in->loc.pos.x + 10, in->loc.pos.y + 41, in->loc.pos.x + 35, in->loc.pos.y + 45, al_map_rgb(0, 0, 0));
            }
        }
        else {
            col = al_map_rgb(128, 128, 128);
            al_draw_filled_rectangle(in->loc.pos.x, in->loc.pos.y, in->loc.pos.x + w, in->loc.pos.y + h, col);
            al_draw_filled_rectangle(in->loc.pos.x + 10, in->loc.pos.y + 41, in->loc.pos.x + 35, in->loc.pos.y + 45, al_map_rgb(0, 0, 0));
        }
    }
    for (int i = 0; i < MAX_PROJETEIS; i++)
        if (projeteis[i].ativo)
            al_draw_filled_circle(projeteis[i].loc.pos.x, projeteis[i].loc.pos.y, 7, al_map_rgb(50, 255, 80));
    for (int i = 0; i < MAX_LIFES; i++)
        if (vidas_extras[i].ativo) {
            if (coracao_preenchido) {
                al_draw_scaled_bitmap(coracao_preenchido,
                    0, 0, al_get_bitmap_width(coracao_preenchido), al_get_bitmap_height(coracao_preenchido),
                    vidas_extras[i].pos.x - 15, vidas_extras[i].pos.y - 15, 30, 30, 0);
            }
            else {
                al_draw_filled_circle(vidas_extras[i].pos.x, vidas_extras[i].pos.y, 14, al_map_rgb(255, 30, 30));
            }
        }
}

