#ifndef ENV_H
#define ENV_H

#include <geometry.h>
#include <graphics.h>

typedef struct {
    COORD pos;
    ANIMATION *anim;
    SPRITE *sprite;
    int collidable;
    HITBOX hitbox;
} PROP;

#endif