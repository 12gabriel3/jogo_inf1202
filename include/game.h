#ifndef GAME_H
#define GAME_H

#include <level.h>
#include <input.h>
#include <menu.h>
#include <allegro5/allegro.h>

#define PAUSE 0
#define PLAY 1

typedef struct {
    int score, n_level;
    int state;
    SPRITE sprites[SPRITES_MAX];
    ANIMATION anims[ANIMS_MAX];
    ALLEGRO_FONT *font;
    LEVEL current_level;
} GAME;

int run_game(GAME *game, ALLEGRO_EVENT event);
int Salva_Jogo(char nome_arquivo_out[MAX_NOME],GAME *game);
int load_jogo(char nome_arquivo_out[MAX_NOME],GAME *game);

#endif
