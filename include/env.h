#ifndef ENV_H
#define ENV_H
#define LINHA 23
#define COLUNA 63
#define MAX_NOME 20
#define MAX_INIMIGOS 100
#define N_Max 1000
#define TROLL 1
#define ZUMBI 2

#include <geometry.h>
#include <graphics.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    ANIMATION *anim;
    SPRITE *sprite;
    COORD pos_graphic;
} ENV;

void update_env(ENV *col);

#endif
