#ifndef GAME_H
#define GAME_H

#include <level.h>
#include <input.h>
#include <allegro5/allegro.h>

#define PAUSE 0
#define PLAY 1

typedef struct {
    int score, n_level;
    int state;
    ALLEGRO_FONT *font;
    LEVEL current_level;
} GAME;

void run_game(GAME *game, ALLEGRO_EVENT event);

#endif