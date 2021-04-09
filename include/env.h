#ifndef ENV_H
#define ENV_H

#include <geometry.h>
#include <graphics.h>

typedef struct {
    ANIMATION *anim;
    SPRITE *sprite;
    COORD pos_graphic;
} ENV;

void update_env(ENV *col);

#endif