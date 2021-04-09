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

typedef enum {MainCharacter, EnemySkeleton, EnemyOgre, EnemySpike} CHARACTER_TYPE; 

typedef struct {
    ANIMATION current;
    ANIMATION *anims;
    int alive;
    CHARACTER_TYPE type;
    COORD pos_graphic, direction;
    HITBOX hitbox;
    float speed;
    int flags;
}CHARACTER;



void update_character(CHARACTER *character, KEYBOARD_STATE kb_state, COORD pos_main);

void set_character_hitbox(CHARACTER *character);

#endif