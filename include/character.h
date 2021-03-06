/*****************************************************************************************
 * OBSERVACAO: Alterar mais tarde as funções para mover os personagens para que
 * tambem recebam o estado do jogo. Assim os inimigos podem seguir o heroi e também
 * é possivel detectar colisoes
 * 
 */

#ifndef CHARACTER_H
#define CHARACTER_H

#include <geometry.h>
#include <graphics.h>
#include <input.h>


#define ANIMS_PER_CHAR 4
#define HITBOX_MARGIN_PX 0
#define FLIP_THRESHOLD 20
#define INV_TIMER 60
#define ATTACK_TIMER 180

typedef enum {MainCharacter, EnemySkeleton, EnemyOgre, EnemySpike} CHARACTER_TYPE; 

typedef struct {
    ANIMATION current;
    ANIMATION *anims;
    int lives;
    int inv_timer, atk_timer;
    CHARACTER_TYPE type;
    COORD pos_graphic, direction;
    CIRCLE hitbox;
    float speed;
    int flags;
}CHARACTER;

typedef struct {
    int active;
    ANIMATION anim;
    COORD pos_graphic;
    CIRCLE hitbox;
} AURA;

void move_character(CHARACTER *character);

void set_character_hitbox(CHARACTER *character);

void set_atk_hitbox(AURA *aura);

#endif