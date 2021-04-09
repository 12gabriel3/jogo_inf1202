#ifndef ENV_H
#define ENV_H

#include <geometry.h>
#include <graphics.h>

typedef struct {
    ANIMATION *anim;
    SPRITE *sprite;
    HITBOX hitbox;
    COORD pos_graphic;
} COLLIDABLE;

#endif