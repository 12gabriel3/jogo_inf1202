#ifndef GEOMETRY_H
#define GEOMETRY_H


#include <input.h>

typedef struct {
    float x, y;
} COORD;

typedef struct{
    int iscircle;
    COORD center;
    float halfheight, halfwidth, radius;
} HITBOX;

/**
 * @brief Converte um estado do teclado em um vetor de movimento
 * 
 * @param input o estado atual do teclado
 * @return COORD Um vetor normalizado que representa a direção do movimento
 */
COORD input_to_vector(KEYBOARD_STATE input);

/**
 * @brief Soma a um vetor um outro vetor de tamanho mod(delta) * speed
 * 
 * @param pos o vetor que será modificado
 * @param delta um vetor que representa o movimento
 * @param speed o fator que o vetor delta será multiplicado
 */
void add_vector(COORD *pos, COORD delta, float speed);


COORD direction_from_to(COORD from, COORD to);


#endif