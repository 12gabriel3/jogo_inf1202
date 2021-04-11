#ifndef GEOMETRY_H
#define GEOMETRY_H


#include <input.h>


#define UP (1 << 0)
#define DOWN (1 << 1)
#define LEFT (1 << 2)
#define RIGHT (1 << 3)

typedef struct {
    float x, y;
} COORD;

typedef struct{
    COORD center;
    float r;
} HITBOX;


typedef struct{
    COORD p1, p2, normal;
} LINE;

/**
 * @brief Converte um estado do teclado em um vetor de movimento
 * 
 * @param input o estado atual do teclado
 * @return COORD Um vetor normalizado que representa a direção do movimento
 */
COORD input_to_vector(KEYBOARD_STATE input);

/**
 * @brief Soma a um vetor a um outro vetor de tamanho mod(delta) * speed
 * 
 * @param pos o vetor que será modificado
 * @param delta um vetor que representa o movimento
 * @param speed o fator que o vetor delta será multiplicado
 */
void add_vector(COORD *pos, COORD delta, float speed);

COORD lc_collision_normal(HITBOX c, LINE l);

int cc_collides(HITBOX c1, HITBOX c2);

float module(COORD v);

COORD direction_from_to(COORD from, COORD to);

COORD dist_from_to(COORD from, COORD to);

COORD cc_collision_normal(HITBOX c1, HITBOX c2);

void rm_direction(COORD direction, COORD *vector);

void vec_sum(COORD *v1, COORD v2);

COORD multiply(float scalar, COORD v);

void normalize(COORD *v);

void set_line_normal(LINE *l, char direction);

#endif