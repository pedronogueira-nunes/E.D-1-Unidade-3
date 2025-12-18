#include "defs.h"
#include "globais.h"
#include "logica.h"
#include "graficos.h"

int main() {
    srand(time(NULL));
    if (!al_init() || !al_install_keyboard() || !al_install_mouse()) {
        fprintf(stderr, "Falha iniciar Allegro\n");
        return -1;
    }
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_init_primitives_addon();

    SessaoJogo* sessao_atual = (SessaoJogo*)malloc(sizeof(SessaoJogo));
    if (sessao_atual) {
        sessao_atual->nome_sessao = (char*)malloc(100 * sizeof(char));
        sessao_atual->id_sessao = rand() % 1000;

        if (sessao_atual->nome_sessao) {
            strcpy(sessao_atual->nome_sessao, "Sistema");
            strcat(sessao_atual->nome_sessao, " Iniciado");
            size_t tam_nome = strlen(sessao_atual->nome_sessao);
            if (strcmp(sessao_atual->nome_sessao, "Sistema Iniciado") == 0) {
                printf("Sessao ID %d: %s (Tam: %zu)\n", sessao_atual->id_sessao, sessao_atual->nome_sessao, tam_nome);
            }
        }
    }

    font = al_load_ttf_font("C:\\Windows\\Fonts\\arial.ttf", 24, 0);
    if (!font) font = al_create_builtin_font();
    timer = al_create_timer(1.0 / 60.0);
    display = al_create_display(SCREEN_W, SCREEN_H);
    queue = al_create_event_queue();
    mapa = al_load_bitmap("mapa.png");
    menu_image = al_load_bitmap("menu.jpg");
    fundo_jogo = al_load_bitmap("fundo_jogo.png");
    customizacao_image = al_load_bitmap("costumizacao.jpeg");

    coracao_preenchido = al_load_bitmap("coracaoPreenchido.png");
    coracao_vazio = al_load_bitmap("coracaoVazio.png");

    inimigo_normal_1 = al_load_bitmap("inimigosNormais_1.png");
    inimigo_normal_2 = al_load_bitmap("inimigosNormais_2.png");
    inimigo_armadura_full = al_load_bitmap("inimigoArmadura_1.png");
    inimigo_armadura_danificada = al_load_bitmap("inimigoArmadura_2.png");
    inimigo_atirador_1 = al_load_bitmap("inimigoAtirador_1.png");
    inimigo_atirador_2 = al_load_bitmap("inimigoAtirador_2.png");
    chefao_pequeno_1 = al_load_bitmap("chefao_1.png");
    chefao_pequeno_2 = al_load_bitmap("chefao_2.png");
    chefao_grande_3 = al_load_bitmap("chefao_3.png");
    chefao_grande_4 = al_load_bitmap("chefao_4.png");

    if (!chefao_pequeno_1 || !chefao_pequeno_2 || !chefao_grande_3 || !chefao_grande_4) {
        fprintf(stderr, "Aviso: Falha ao carregar uma ou mais imagens do Chef�o. Usando fallback primitivo.\n");
    }

    inimigo_pulador_chao = al_load_bitmap("inimigoPulo_1.png");
    inimigo_pulador_pulo = al_load_bitmap("inimigoPulo_2.png");
    if (!inimigo_pulador_chao || !inimigo_pulador_pulo) {
        fprintf(stderr, "Aviso: Falha ao carregar uma ou mais imagens do Inimigo Pulador. Usando fallback primitivo.\n");
    }

    teclas_wasd_image = al_load_bitmap("letrasTeclado.jpeg");
    teclas_setas_image = al_load_bitmap("setasTeclado.jpeg");
    if (!teclas_wasd_image || !teclas_setas_image) {
        fprintf(stderr, "Aviso: Falha ao carregar uma ou mais imagens de controle. Usando fallback primitivo.\n");
    }

    sprites_animacao_sheets[0] = al_load_bitmap("sprites_1.png");
    sprites_animacao_sheets[1] = al_load_bitmap("sprites_2.png");
    sprites_animacao_sheets[2] = al_load_bitmap("sprites_3.png");
    sprites_animacao_sheets[3] = al_load_bitmap("sprites_4.png");

    skin_images[0] = al_load_bitmap("skin_1.png");
    skin_images[1] = al_load_bitmap("skin_2.png");
    skin_images[2] = al_load_bitmap("skin_3.png");
    skin_images[3] = al_load_bitmap("skin_4.png");

    if (skin_images[0]) {
        skin_atual_jogador = skin_images[0];
        sprites_animacao_atual = sprites_animacao_sheets[0];
    }

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());

    al_start_timer(timer);
    Inimigo* inimigos = (Inimigo*)malloc(MAX_INIMIGOS * sizeof(Inimigo));
    Projetil* projeteis = (Projetil*)malloc(MAX_PROJETEIS * sizeof(Projetil));
    VidaExtra* vidas_extras = (VidaExtra*)malloc(MAX_LIFES * sizeof(VidaExtra));
    if (!inimigos || !projeteis || !vidas_extras) {
        fprintf(stderr, "Falha na alocacao de memoria error\n");
        return -1;
    }

    memset(projeteis, 0, MAX_PROJETEIS * sizeof(Projetil));
    memset(vidas_extras, 0, MAX_LIFES * sizeof(VidaExtra));

    bool sair = false;
    EstadoJogo estado = MENU;
    ALLEGRO_EVENT event;
    Personagem p = { 0 };
    p.loc.pos.x = SCREEN_W / 2 - 15;
    p.loc.pos.y = NIVEL_H - 60;
    p.loc.vel.x = 0;
    p.loc.vel.y = 0;
    p.no_chao = false;
    p.dir_x = 0;
    p.frame_atual = 0;
    p.anim_timer = 0.0f;

    nive_ljogando = 0; vidas = VIDAS_INICIAIS; pontos = 0;
    selecao_gameover = 0; pontos_suficientes = false;
    resetar_inimigos(inimigos, nive_ljogando);
    int botao_w = 160, botao_h = 38;
    int botao_x = SCREEN_W - botao_w - 20;
    int botao_y = MAPA_OFFSET_Y - botao_h - 10;
    int inimigo_anim_timer = 0;
    while (!sair) {
        al_wait_for_event(queue, &event);
        if (estado == JOGO) {
            tempo_jogo += al_get_timer_speed(timer);
            if (inimigo_anim_timer++ >= 120) {
                frame_animacao_inimigo = (frame_animacao_inimigo == 0) ?
                    1 : 0;
                inimigo_anim_timer = 0;
            }

            if (inimigos[0].ativo && inimigos[0].tipo == INIMIGO_CHEFAO) {
                Inimigo* chefe = &inimigos[0];
                int troca_frames = chefe->invulneravel ? 120 : 60;

                if (chefao_anim_timer++ >= troca_frames) {
                    chefao_anim_frame = (chefao_anim_frame == 0) ?
                        1 : 0;
                    chefao_anim_timer = 0;
                }
            }

        }

        if (event.type == ALLEGRO_EVENT_TIMER) {
            if (estado == MENU)
                desenhar_menu();
            else if (estado == CUSTOM)
                desenhar_customizacao();
            else if (estado == CONFIG)
                desenhar_configuracao();
            else if (estado == MAPA)
            {
                if (stats_mostrando) {
                    al_clear_to_color(al_map_rgb(20, 20, 30));
                    desenhar_stats();
                }
                else
                    desenhar_mapa_niveis();
            }
            else if (estado == GAMEOVER)
                desenhar_gameover(pontos, pontos_suficientes, selecao_gameover, nive_ljogando);
            else if (estado == JOGO)
                desenhar_jogo(&p, vidas, inimigos, vidas_extras, projeteis, pontos, pontos_suficientes, nive_ljogando);
            al_flip_display();
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            if (estado == MAPA) {
                nivel_hover = -1;
                if (!stats_mostrando) {
                    for (int i = 0; i < total_niveis; i++)
                        if (niveis[i].desbloqueado && dist(event.mouse.x, event.mouse.y, niveis[i].x, niveis[i].y) < 18) nivel_hover = i;
                }
            }
            else if (estado == MENU && menu_image) {
                int mx = event.mouse.x, my = event.mouse.y;
                selecao = -1;

                for (int i = 0; i < 3; i++) {
                    float x1 = MENU_DRAW_COORDS[i][0] * SCREEN_W / 1350.0f;
                    float x2 = MENU_DRAW_COORDS[i][1] * SCREEN_W / 1350.0f;
                    float y1 = MENU_DRAW_COORDS[i][2] * SCREEN_H / 800.0f;
                    float y2 = MENU_DRAW_COORDS[i][3] * SCREEN_H / 800.0f;

                    if (mx >= x1 && mx <= x2 && my >= y1 && my <= y2) {
                        selecao = i;
                        break;
                    }
                }
            }
            else if (estado == CUSTOM) {
                int mx = event.mouse.x, my = event.mouse.y;
                selecao_customizacao = -1;
                for (int i = 0; i < 4; i++) {
                    float x1 = CUSTOM_SELECT_COORDS[i][0] * SCREEN_W / 1350.0f;
                    float x2 = CUSTOM_SELECT_COORDS[i][1] * SCREEN_W / 1350.0f;
                    float y1 = CUSTOM_SELECT_COORDS[i][2] * SCREEN_H / 800.0f;
                    float y2 = CUSTOM_SELECT_COORDS[i][3] * SCREEN_H / 800.0f;

                    if (mx >= x1 && mx <= x2 && my >= y1 && my <= y2) {
                        selecao_customizacao = i;
                        break;
                    }
                }
            }
            else if (estado == CONFIG) {
                int mx = event.mouse.x, my = event.mouse.y;
                selecao_configuracao = -1;
                for (int i = 0; i < 2; i++) {
                    float x1 = CONFIG_SELECT_COORDS[i][0] * SCREEN_W / 1350.0f;
                    float x2 = CONFIG_SELECT_COORDS[i][1] * SCREEN_W / 1350.0f;
                    float y1 = CONFIG_SELECT_COORDS[i][2] * SCREEN_H / 800.0f;
                    float y2 = CONFIG_SELECT_COORDS[i][3] * SCREEN_H / 800.0f;

                    if (mx >= x1 && mx <= x2 && my >= y1 && my <= y2) {
                        selecao_configuracao = i;
                        break;
                    }
                }
            }
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            int mx = event.mouse.x, my = event.mouse.y;
            if (estado == MAPA) {
                if (mx > botao_x && mx < botao_x + botao_w && my > botao_y && my < botao_y + botao_h)
                    stats_mostrando = true;
                else if (nivel_hover >= 0 && niveis[nivel_hover].desbloqueado && !stats_mostrando) {
                    p.loc.pos.x = SCREEN_W / 2 - 15;
                    p.loc.pos.y = NIVEL_H - PLAYER_DRAW_H; p.loc.vel.x = p.loc.vel.y = 0; p.no_chao = false;
                    p.dir_x = 0; p.frame_atual = 0;
                    p.anim_timer = 0.0f;
                    nive_ljogando = nivel_hover;
                    vidas = VIDAS_INICIAIS; pontos = 0; pontos_suficientes = false;
                    resetar_inimigos(inimigos, nive_ljogando);
                    empilhar_estado(estado);
                    estado = JOGO;
                    dano_timer = 0; invulneravel = false;
                    memset(projeteis, 0, MAX_PROJETEIS * sizeof(Projetil));
                    memset(vidas_extras, 0, MAX_LIFES * sizeof(VidaExtra));
                    life_spawn_timer = 0;
                    chefao_anim_timer = 0; chefao_anim_frame = 0;
                }
            }
            else if (estado == MENU && menu_image) {
                for (int i = 0; i < 3; i++) {
                    float x1 = MENU_DRAW_COORDS[i][0] * SCREEN_W / 1350.0f;
                    float x2 = MENU_DRAW_COORDS[i][1] * SCREEN_W / 1350.0f;
                    float y1 = MENU_DRAW_COORDS[i][2] * SCREEN_H / 800.0f;
                    float y2 = MENU_DRAW_COORDS[i][3] * SCREEN_H / 800.0f;

                    if (mx >= x1 && mx <= x2 && my >= y1 && my <= y2) {
                        selecao = i;
                        if (selecao == 0) {
                            empilhar_estado(estado); estado = MAPA;
                        }
                        else if (selecao == 1) {
                            empilhar_estado(estado); estado = CUSTOM;
                        }
                        else if (selecao == 2) {
                            empilhar_estado(estado); estado = CONFIG;
                        }
                        break;
                    }
                }
            }
            else if (estado == CUSTOM) {
                if (selecao_customizacao >= 0) {
                    printf("Opcao de Customizacao %d selecionada!\n", selecao_customizacao
                        + 1);
                    if (skin_images[selecao_customizacao]) {
                        skin_atual_jogador = skin_images[selecao_customizacao];
                        sprites_animacao_atual = sprites_animacao_sheets[selecao_customizacao];
                        skin_selecionada_idx = selecao_customizacao;
                    }
                }
            }
            else if (estado == CONFIG) {
                if (selecao_configuracao >= 0) {
                    controle_selecionado_idx = selecao_configuracao;
                    printf("Esquema de controle selecionado: %s\n", (controle_selecionado_idx == 0) ? "WASD" : "SETAS");
                }
            }
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (estado == MENU) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) sair = true;
            }
            else if (estado == MAPA) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE && !stats_mostrando) estado = desempilhar_estado();
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE && stats_mostrando) stats_mostrando = false;
            }
            else if (estado == CUSTOM) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    estado = desempilhar_estado();
                }
            }
            else if (estado == CONFIG) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    estado = desempilhar_estado();
                }
            }
            else if (estado == GAMEOVER) {
                bool proximo_liberado_input = (nive_ljogando < total_niveis - 1) && (niveis[nive_ljogando + 1].desbloqueado);
                int num_opcoes = proximo_liberado_input ? 3 : 2;
                if (event.keyboard.keycode == ALLEGRO_KEY_S || event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                    selecao_gameover = (selecao_gameover + 1) % num_opcoes;
                else if (event.keyboard.keycode == ALLEGRO_KEY_W || event.keyboard.keycode == ALLEGRO_KEY_UP)
                    selecao_gameover = (selecao_gameover - 1 + num_opcoes) % num_opcoes;
                else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER || event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER) {
                    if (proximo_liberado_input) {
                        if (selecao_gameover == 0) {
                            vidas = VIDAS_INICIAIS;
                            pontos = 0;
                            p.loc.pos.x = SCREEN_W / 2 - 15;
                            p.loc.pos.y = NIVEL_H - PLAYER_DRAW_H; p.loc.vel.x = p.loc.vel.y = 0;
                            p.no_chao = false;
                            p.dir_x = 0; p.frame_atual = 0;
                            p.anim_timer = 0.0f;
                            resetar_inimigos(inimigos, nive_ljogando);
                            empilhar_estado(estado);
                            estado = JOGO; pontos_suficientes = false;
                            dano_timer = 0; invulneravel = false;
                            memset(projeteis, 0, MAX_PROJETEIS * sizeof(Projetil));
                            memset(vidas_extras, 0, MAX_LIFES * sizeof(VidaExtra));
                            life_spawn_timer = 0;
                            chefao_anim_timer = 0; chefao_anim_frame = 0;
                        }
                        else if (selecao_gameover == 1) {
                            if (nive_ljogando < total_niveis - 1) {

                                nive_ljogando++;
                                vidas = VIDAS_INICIAIS; pontos = 0;
                                p.loc.pos.x = SCREEN_W / 2 - 15;
                                p.loc.pos.y = NIVEL_H - PLAYER_DRAW_H;
                                p.loc.vel.x = p.loc.vel.y = 0; p.no_chao = false;
                                p.dir_x = 0; p.frame_atual = 0;
                                p.anim_timer = 0.0f;
                                niveis[nive_ljogando].desbloqueado = true;
                                resetar_inimigos(inimigos, nive_ljogando);
                                empilhar_estado(estado);
                                estado = JOGO; pontos_suficientes = false; dano_timer = 0; invulneravel = false;
                                memset(projeteis, 0, MAX_PROJETEIS * sizeof(Projetil));
                                memset(vidas_extras, 0, MAX_LIFES * sizeof(VidaExtra));
                                life_spawn_timer = 0;
                                chefao_anim_timer = 0; chefao_anim_frame = 0;
                            }
                        }
                        else if (selecao_gameover == 2) {
                            empilhar_estado(estado);
                            estado = MAPA;
                        }
                    }
                    else {
                        if (selecao_gameover == 0) {
                            vidas = VIDAS_INICIAIS;
                            pontos = 0;
                            p.loc.pos.x = SCREEN_W / 2 - 15;
                            p.loc.pos.y = NIVEL_H - PLAYER_DRAW_H; p.loc.vel.x = p.loc.vel.y = 0;
                            p.no_chao = false;
                            p.dir_x = 0; p.frame_atual = 0;
                            p.anim_timer = 0.0f;
                            resetar_inimigos(inimigos, nive_ljogando);
                            empilhar_estado(estado);
                            estado = JOGO; pontos_suficientes = false;
                            dano_timer = 0; invulneravel = false;
                            memset(projeteis, 0, MAX_PROJETEIS * sizeof(Projetil));
                            memset(vidas_extras, 0, MAX_LIFES * sizeof(VidaExtra));
                            life_spawn_timer = 0;
                            chefao_anim_timer = 0; chefao_anim_frame = 0;
                        }
                        else if (selecao_gameover == 1) {
                            empilhar_estado(estado);
                            estado = MAPA;
                        }
                    }
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) estado = desempilhar_estado();
            }
            else if (estado == JOGO) {
                int vel_h = 6;
                int vel_v = 12;

                if (nive_ljogando == MAX_NIVEIS - 1) {
                    vel_h = 4 + nive_ljogando;
                    vel_v = 10 + nive_ljogando;
                }

                if (controle_selecionado_idx == 0) {
                    if (event.keyboard.keycode == ALLEGRO_KEY_A) p.loc.vel.x = -vel_h;
                    if (event.keyboard.keycode == ALLEGRO_KEY_D) p.loc.vel.x = vel_h;
                    if (event.keyboard.keycode == ALLEGRO_KEY_W && p.no_chao) {
                        p.loc.vel.y = -vel_v;
                        p.no_chao = false;
                    }

                }
                else if (controle_selecionado_idx == 1) {
                    if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) p.loc.vel.x = -vel_h;
                    if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) p.loc.vel.x = vel_h;
                    if (event.keyboard.keycode == ALLEGRO_KEY_UP && p.no_chao) {
                        p.loc.vel.y = -vel_v;
                        p.no_chao = false;
                    }
                }

                if (event.keyboard.keycode == ALLEGRO_KEY_SPACE && p.no_chao) {
                    p.loc.vel.y = -vel_v;
                    p.no_chao = false;
                }

                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) estado = desempilhar_estado();
            }
        }
        else if (event.type == ALLEGRO_EVENT_KEY_UP && estado == JOGO) {
            if (controle_selecionado_idx == 0) {
                if (event.keyboard.keycode == ALLEGRO_KEY_A) {
                    if (p.loc.vel.x < 0) p.loc.vel.x = 0;
                }
                if (event.keyboard.keycode == ALLEGRO_KEY_D) {
                    if (p.loc.vel.x > 0) p.loc.vel.x = 0;
                }
            }
            else if (controle_selecionado_idx == 1) {
                if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                    if (p.loc.vel.x < 0) p.loc.vel.x = 0;
                }
                if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                    if (p.loc.vel.x > 0) p.loc.vel.x = 0;
                }
            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) sair = true;
        if (estado == JOGO) {

            if (nive_ljogando == MAX_NIVEIS - 1) {
                if (life_spawn_timer <= 0) {
                    int slot = -1;
                    for (int i = 0; i < MAX_LIFES; i++) {
                        if (!vidas_extras[i].ativo) {
                            slot = i;
                            break;
                        }
                    }

                    if (slot != -1 && (rand() % 100) < 10) {
                        vidas_extras[slot].ativo = true;
                        vidas_extras[slot].pos.x = rand() % (SCREEN_W - 80) + 40;
                        vidas_extras[slot].pos.y = NIVEL_H - 20;
                        life_spawn_timer = 180;
                    }
                    else {
                        life_spawn_timer = 30;
                    }
                }
                else {
                    life_spawn_timer--;
                }
            }

            p.loc.vel.y += 0.6f;
            p.loc.pos.x += p.loc.vel.x;
            p.loc.pos.y += p.loc.vel.y;
            if (p.loc.pos.x < 0) p.loc.pos.x = 0;
            if (p.loc.pos.x > SCREEN_W - PLAYER_DRAW_W) p.loc.pos.x = SCREEN_W - PLAYER_DRAW_W;
            if (p.loc.pos.y > NIVEL_H - PLAYER_DRAW_H) {
                p.loc.pos.y = NIVEL_H - PLAYER_DRAW_H;
                p.loc.vel.y = 0;
            }

            bool tocou = check_colisao(p.loc.pos.x, p.loc.pos.y);
            if (tocou) {
                p.loc.pos.y -= p.loc.vel.y;
                p.loc.vel.y = 0; p.no_chao = true;
            }
            else p.no_chao = false;
            if (p.loc.vel.x != 0) {
                p.dir_x = (p.loc.vel.x > 0) ?
                    1 : -1;
                p.anim_timer += fabs(p.loc.vel.x) * 0.2f;
                if (p.anim_timer >= 1.0f) {
                    p.frame_atual = (p.frame_atual + 1) % 5;
                    p.anim_timer -= 1.0f;
                }
            }
            else {
                p.dir_x = 0;
                p.frame_atual = 0;
                p.anim_timer = 0.0f;
            }

            for (int i = 0; i < MAX_PROJETEIS; i++) {
                if (!projeteis[i].ativo) continue;
                projeteis[i].loc.pos.x += projeteis[i].loc.vel.x;
                projeteis[i].loc.pos.y += projeteis[i].loc.vel.y;
                if (projeteis[i].loc.pos.x < -32 || projeteis[i].loc.pos.x > SCREEN_W + 32 || projeteis[i].loc.pos.y < 0 || projeteis[i].loc.pos.y > SCREEN_H)
                    projeteis[i].ativo = false;
                if (projeteis[i].ativo && !invulneravel &&
                    p.loc.pos.x + PLAYER_DRAW_W > projeteis[i].loc.pos.x && p.loc.pos.x < projeteis[i].loc.pos.x + 10 &&
                    p.loc.pos.y + PLAYER_DRAW_H > projeteis[i].loc.pos.y && p.loc.pos.y < projeteis[i].loc.pos.y + 10) {
                    projeteis[i].ativo = false;
                    vidas--;
                    dano_timer = 35; invulneravel = true;
                    if (vidas <= 0) {
                        vezes_derrotado++;
                        if (nive_ljogando < total_niveis - 1) {
                            if (pontos >= pontos_necessarios[nive_ljogando])
                                niveis[nive_ljogando + 1].desbloqueado = true;
                            pontos_suficientes = pontos >= pontos_necessarios[nive_ljogando];
                        }
                        else {
                            pontos_suficientes = false;
                        }
                        empilhar_estado(estado);
                        estado = GAMEOVER;
                        selecao_gameover = 0; dano_timer = 0; invulneravel = false;
                        if (pontos > ultimas_pontuacoes[nive_ljogando]) {
                            ultimas_pontuacoes[nive_ljogando] = pontos;
                            tempos_por_nivel[nive_ljogando] = tempo_jogo;
                        }
                    }
                }
            }

            for (int i = 0; i < MAX_INIMIGOS; i++) {
                if (!inimigos[i].ativo) continue;
                Inimigo* in = &inimigos[i];

                if (in->tipo == INIMIGO_CHEFAO) {

                    if (in->recalc_tick > 0) {
                        in->recalc_tick--;
                        in->scale_dir = 0;

                        if (in->recalc_tick == 0) {
                            if (in->scale >= 3.0f) {
                                in->scale_dir = -1;
                            }
                            else if (in->scale <= 1.5f) {
                                in->scale_dir = 1;
                            }
                        }
                    }

                    in->scale += in->scale_dir * 0.04f;
                    if (in->scale >= 3.0f && in->scale_dir > 0) {
                        in->scale = 3.0f;
                        in->scale_dir = 0;
                        in->recalc_tick = 180;
                        in->invulneravel = true;
                        in->dir *= -1;
                        in->patrulha_tick = 60 + rand() % 30;
                    }
                    else if (in->scale <= 1.5f && in->scale_dir < 0) {
                        in->scale = 1.5f;
                        in->scale_dir = 0;
                        in->recalc_tick = 120;
                        in->invulneravel = false;
                        in->patrulha_tick = 40 + rand() % 20;
                        int spawn_count = 0;
                        float spawn_x_base = in->loc.pos.x + (28 * in->scale) / 2.0f;
                        float spawn_y = NIVEL_H - 45;
                        for (int k = 1; k < MAX_INIMIGOS && spawn_count < 3; k++) {
                            if (!inimigos[k].ativo) {
                                Inimigo* minion = &inimigos[k];
                                minion->ativo = true;
                                minion->loc.pos.x = spawn_x_base - 22;
                                minion->loc.pos.y = spawn_y;
                                minion->loc.vel.x = 0;
                                minion->loc.vel.y = 0;
                                minion->dir = (rand() % 2 == 0) ? 1 : -1;
                                TipoInimigo tipo;
                                int tipo_idx = rand() % 10;
                                if (tipo_idx >= 0 && tipo_idx <= 3) tipo = INIMIGO_NORMAL;
                                else if (tipo_idx >= 4 && tipo_idx <= 7) tipo = INIMIGO_PULADOR;
                                else if (tipo_idx == 8) tipo = INIMIGO_ARMADURA;
                                else tipo = INIMIGO_ATIRADOR;

                                minion->tipo = tipo;
                                minion->vida_armadura = (tipo == INIMIGO_ARMADURA) ? 2 : 0;
                                minion->speed = 1.0f + (rand() % 50) * 0.02f;
                                minion->aggro = 0;
                                minion->recalc_tick = 0;
                                minion->patrulha_tick = 60;
                                minion->salto_duplo = 0;
                                minion->persegue_jogador = (rand() % 2 == 0);

                                spawn_count++;
                            }
                        }
                    }

                    if (in->patrulha_tick-- <= 0) {
                        in->dir = (p.loc.pos.x < in->loc.pos.x) ?
                            -1 : 1;
                        in->patrulha_tick = 30 + rand() % 30;
                    }

                    in->loc.vel.x = in->dir * in->speed * (in->scale > 2.25f ? 0.4f : 1.8f);
                    in->loc.pos.x += in->loc.vel.x;

                    if (in->loc.pos.y + 28 * in->scale < NIVEL_H)
                        in->loc.vel.y += 0.7f + 0.01f * nive_ljogando;
                    else {
                        in->loc.pos.y = NIVEL_H - 28 * in->scale;
                        in->loc.vel.y = 0;
                        in->loc.vel.x *= 0.8f;
                    }
                    in->loc.pos.y += in->loc.vel.y;
                    if (in->loc.pos.x < 0) {
                        in->loc.pos.x = 0;
                        in->dir = 1;
                    }
                    if (in->loc.pos.x > SCREEN_W - 28 * in->scale) {
                        in->loc.pos.x = SCREEN_W - 28 * in->scale;
                        in->dir = -1;
                    }

                    if (in->vida_chefo <= 0) {
                        in->ativo = false;
                        pontos += 500;
                        pontuacao_total += 500;
                        if (pontos > pontuacoes_maximas[nive_ljogando])
                            pontuacoes_maximas[nive_ljogando] = pontos;
                        inimigos_derrotados++;

                        pontos_suficientes = true;
                        empilhar_estado(estado);
                        estado = GAMEOVER;
                        selecao_gameover = 0; dano_timer = 0; invulneravel = false;
                    }
                    continue;
                }

                if (in->tipo == INIMIGO_ATIRADOR && nive_ljogando >= 3) {
                    in->loc.vel.x = in->dir * in->speed;
                    in->loc.pos.x += in->loc.vel.x;
                    if (in->loc.pos.y + 45 < NIVEL_H)
                        in->loc.vel.y += 0.7f + 0.01f * nive_ljogando;
                    else {
                        in->loc.pos.y = NIVEL_H - 45;
                        in->loc.vel.y = 0;
                    }
                    in->loc.pos.y += in->loc.vel.y;
                    if (in->loc.pos.x < 0) {
                        in->loc.pos.x = 0;
                        in->dir = 1;
                    }
                    if (in->loc.pos.x > SCREEN_W - 45) {
                        in->loc.pos.x = SCREEN_W - 45;
                        in->dir = -1;
                    }

                    if (--in->aggro <= 0) {
                        float vel = 3.5f + nive_ljogando * 0.5f;
                        int new_aggro = 65 + rand() % 55;

                        if (nive_ljogando == MAX_NIVEIS - 1) {
                            vel = 5.0f;
                            new_aggro = 90 + rand() % 60;
                        }

                        if (nive_ljogando == 3) {
                            float dir = in->dir;
                            cria_projetil(projeteis, in->loc.pos.x + 14, in->loc.pos.y + 14, in->loc.pos.x + 14 + dir * 12, in->loc.pos.y + 14, vel);
                        }
                        else {
                            cria_projetil(projeteis, in->loc.pos.x + 14, in->loc.pos.y + 14, p.loc.pos.x + PLAYER_DRAW_W / 2, p.loc.pos.y + PLAYER_DRAW_H / 2, vel);
                        }
                        in->aggro = new_aggro;
                    }
                    continue;
                }

                if (in->tipo == INIMIGO_PULADOR) {
                    if (in->loc.pos.y + 45 >= NIVEL_H && in->loc.vel.y == 0) {
                        if (in->recalc_tick-- <= 0) {

                            if (rand() % 7 == 0) in->dir *= -1;
                            in->loc.vel.y = -8.5f - 1.2f * nive_ljogando + (rand() % 20) * 0.03f;
                            in->loc.vel.x = in->dir * (1.6f + (rand() % 10) * 0.15f + 0.17f * nive_ljogando);
                            in->recalc_tick = 18 + rand() % 16;
                        }
                    }
                    in->loc.pos.x += in->loc.vel.x;
                    in->loc.pos.y += in->loc.vel.y;
                    in->loc.vel.y += 0.7f + 0.03f * nive_ljogando;
                    if (in->loc.pos.y + 45 >= NIVEL_H) {
                        in->loc.pos.y = NIVEL_H - 45;
                        in->loc.vel.y = 0;
                        in->loc.vel.x = 0;
                    }

                    if (in->loc.pos.x < 0) {
                        in->loc.pos.x = 0;
                        in->dir = 1;
                    }
                    if (in->loc.pos.x > SCREEN_W - 45) {
                        in->loc.pos.x = SCREEN_W - 45;
                        in->dir = -1;
                    }

                    continue;
                }

                in->patrulha_tick--;
                if (in->patrulha_tick <= 0) {
                    if (in->tipo == INIMIGO_ARMADURA && in->loc.vel.y == 0 && (rand() % 3) == 0)
                        in->loc.vel.y = -8.2f - 0.7f * nive_ljogando;
                    else if ((rand() % 3) == 0)
                        in->dir *= -1;
                    in->patrulha_tick = 40 + rand() % 70;
                }
                in->recalc_tick--;
                if (in->recalc_tick <= 0 && in->tipo == INIMIGO_NORMAL && in->persegue_jogador) {
                    float dx = p.loc.pos.x - in->loc.pos.x;
                    if (fabs(dx) < 175) in->dir = (dx < 0) ? -1 : 1;
                    in->speed = (2.3f + rand() % 25 * 0.03f) + 0.11f * nive_ljogando;
                    in->recalc_tick = 18 + rand() % 55;
                }
                else if (in->recalc_tick <= 0) {
                    in->speed = (1.0f + 0.11f * nive_ljogando) + (rand() % 70) * 0.012f;
                    in->recalc_tick = 15 + rand() % 50;
                }
                if (in->persegue_jogador && (rand() % 4) == 0)
                    in->loc.vel.x = in->dir * in->speed * 1.6f;
                else
                    in->loc.vel.x = in->dir * in->speed;
                in->loc.pos.x += in->loc.vel.x;
                if (in->loc.pos.y + 45 < NIVEL_H)
                    in->loc.vel.y += 0.7f + 0.01f * nive_ljogando;
                else {
                    in->loc.pos.y = NIVEL_H - 45;
                    in->loc.vel.y = 0;
                }
                in->loc.pos.y += in->loc.vel.y;
                if (in->loc.pos.x < 0) {
                    in->loc.pos.x = 0;
                    in->dir = 1;
                }
                if (in->loc.pos.x > SCREEN_W - 45) {
                    in->loc.pos.x = SCREEN_W - 45;
                    in->dir = -1;
                }

            }

            for (int i = 0; i < MAX_INIMIGOS; i++) {
                if (!inimigos[i].ativo) continue;
                Inimigo* in = &inimigos[i];

                float enemy_w;
                float enemy_h;
                if (in->tipo == INIMIGO_CHEFAO) {
                    enemy_w = 28 * in->scale;
                    enemy_h = 28 * in->scale;
                }
                else {
                    enemy_w = 45;
                    enemy_h = 45;
                }

                bool col_x = p.loc.pos.x + PLAYER_DRAW_W > in->loc.pos.x && p.loc.pos.x < in->loc.pos.x + enemy_w;
                bool col_y = p.loc.pos.y + PLAYER_DRAW_H > in->loc.pos.y && p.loc.pos.y < in->loc.pos.y + enemy_h;
                if (col_x && col_y && !invulneravel) {
                    bool por_cima = (p.loc.vel.y > 0) && ((p.loc.pos.y + PLAYER_DRAW_H) - in->loc.pos.y <= 15);
                    if (in->tipo == INIMIGO_ARMADURA && nive_ljogando == MAX_NIVEIS - 1) {
                        por_cima = (p.loc.vel.y > 0) && ((p.loc.pos.y + PLAYER_DRAW_H) - in->loc.pos.y <= 25);
                    }

                    if (in->tipo == INIMIGO_CHEFAO) {
                        if (por_cima) {
                            if (!in->invulneravel) {

                                in->vida_chefo--;
                                p.loc.vel.y = -12;
                            }
                            else {
                                vidas--;
                                dano_timer = 35; invulneravel = true;
                                p.loc.vel.y = -5;
                            }
                        }
                        else {
                            vidas--;
                            dano_timer = 35; invulneravel = true;
                        }

                        if (vidas <= 0) {
                            vezes_derrotado++;
                            pontos_suficientes = false;
                            empilhar_estado(estado);
                            estado = GAMEOVER; selecao_gameover = 0; dano_timer = 0; invulneravel = false;
                            if (pontos > ultimas_pontuacoes[nive_ljogando]) {
                                ultimas_pontuacoes[nive_ljogando] = pontos;
                                tempos_por_nivel[nive_ljogando] = tempo_jogo;
                            }
                        }
                        break;
                    }
                    else if (por_cima) {
                        if (in->tipo == INIMIGO_ARMADURA) {
                            if (in->vida_armadura == 2) {

                                in->vida_armadura = 1;
                                p.loc.vel.y = -10;
                            }
                            else {
                                in->ativo = false;
                                pontos += 100;
                                pontuacao_total += 100;
                                if (pontos > pontuacoes_maximas[nive_ljogando])
                                    pontuacoes_maximas[nive_ljogando] = pontos;
                                inimigos_derrotados++;

                                int rebote_v = -12;
                                if (nive_ljogando == MAX_NIVEIS - 1) {
                                    rebote_v = -12 - nive_ljogando / 2;
                                }
                                p.loc.vel.y = rebote_v;
                                respawn_inimigo(in, nive_ljogando);
                            }
                        }
                        else {
                            in->ativo = false;
                            pontos += 100;
                            pontuacao_total += 100;
                            if (pontos > pontuacoes_maximas[nive_ljogando])
                                pontuacoes_maximas[nive_ljogando] = pontos;
                            inimigos_derrotados++;

                            int rebote_v = -12;
                            if (nive_ljogando == MAX_NIVEIS - 1) {
                                rebote_v = -12 - nive_ljogando / 2;
                            }
                            p.loc.vel.y = rebote_v;
                            respawn_inimigo(in, nive_ljogando);
                        }
                        break;
                    }
                    else {
                        vidas--;
                        dano_timer = 35;
                        invulneravel = true;

                        if (vidas <= 0) {
                            vezes_derrotado++;
                            if (nive_ljogando < total_niveis - 1) {
                                if (pontos >= pontos_necessarios[nive_ljogando])
                                    niveis[nive_ljogando + 1].desbloqueado = true;
                                pontos_suficientes = pontos >= pontos_necessarios[nive_ljogando];
                            }
                            else {
                                pontos_suficientes = false;
                            }
                            empilhar_estado(estado);
                            estado = GAMEOVER;
                            selecao_gameover = 0; dano_timer = 0; invulneravel = false;
                            if (pontos > ultimas_pontuacoes[nive_ljogando]) {
                                ultimas_pontuacoes[nive_ljogando] = pontos;
                                tempos_por_nivel[nive_ljogando] = tempo_jogo;
                            }
                        }
                        break;
                    }
                }
            }

            for (int i = 0; i < MAX_LIFES; i++) {
                if (vidas_extras[i].ativo) {
                    float life_x = vidas_extras[i].pos.x;
                    float life_y = vidas_extras[i].pos.y;
                    float life_r = 14;
                    if (p.loc.pos.x + PLAYER_DRAW_W > life_x - life_r && p.loc.pos.x < life_x + life_r &&
                        p.loc.pos.y + PLAYER_DRAW_H > life_y - life_r && p.loc.pos.y < life_y + life_r)
                    {

                        vidas_extras[i].ativo = false;
                        if (vidas < VIDAS_INICIAIS) {
                            vidas++;
                        }
                        break;
                    }
                }
            }

            if (nive_ljogando == MAX_NIVEIS - 1) {
            }
            else {
                pontos_suficientes = pontos >= pontos_necessarios[nive_ljogando];
            }

            if (dano_timer > 0) {
                dano_timer--;
                if (dano_timer == 0) invulneravel = false;
            }
        }
    }

    free(inimigos);
    free(projeteis);
    free(vidas_extras);

    salvar_estatisticas_em_arquivo();

    if (sessao_atual) {
        if (sessao_atual->nome_sessao) free(sessao_atual->nome_sessao);
        free(sessao_atual);
    }

    if (menu_image) al_destroy_bitmap(menu_image);
    if (fundo_jogo) al_destroy_bitmap(fundo_jogo);
    if (customizacao_image) al_destroy_bitmap(customizacao_image);

    if (coracao_preenchido) al_destroy_bitmap(coracao_preenchido);
    if (coracao_vazio) al_destroy_bitmap(coracao_vazio);
    if (inimigo_normal_1) al_destroy_bitmap(inimigo_normal_1);
    if (inimigo_normal_2) al_destroy_bitmap(inimigo_normal_2);
    if (inimigo_armadura_full) al_destroy_bitmap(inimigo_armadura_full);
    if (inimigo_armadura_danificada) al_destroy_bitmap(inimigo_armadura_danificada);
    if (inimigo_atirador_1) al_destroy_bitmap(inimigo_atirador_1);
    if (inimigo_atirador_2) al_destroy_bitmap(inimigo_atirador_2);
    if (chefao_pequeno_1) al_destroy_bitmap(chefao_pequeno_1);
    if (chefao_pequeno_2) al_destroy_bitmap(chefao_pequeno_2);
    if (chefao_grande_3) al_destroy_bitmap(chefao_grande_3);
    if (chefao_grande_4) al_destroy_bitmap(chefao_grande_4);

    if (inimigo_pulador_chao) al_destroy_bitmap(inimigo_pulador_chao);
    if (inimigo_pulador_pulo) al_destroy_bitmap(inimigo_pulador_pulo);
    if (teclas_wasd_image) al_destroy_bitmap(teclas_wasd_image);
    if (teclas_setas_image) al_destroy_bitmap(teclas_setas_image);
    for (int i = 0; i < 4; i++) {
        if (sprites_animacao_sheets[i]) al_destroy_bitmap(sprites_animacao_sheets[i]);
    }

    for (int i = 0; i < 4; i++) {
        if (skin_images[i]) al_destroy_bitmap(skin_images[i]);
    }

    if (mapa) al_destroy_bitmap(mapa);
    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(queue);
    return 0;
}
