#ifndef LEVEL_H
#define LEVEL_H

#include <env.h>
#include <geometry.h>
#include <character.h>

#define ENVS_MAX 200
#define CHARACTERS_MAX 50
#define LINES_MAX 100

typedef struct {
    ENV envs[ENVS_MAX];
    int n_envs;
    CHARACTER characters[CHARACTERS_MAX];
    int n_characters;
    LINE lines[LINES_MAX];
    int n_lines;
    KEYBOARD_STATE input;
} LEVEL;

void set_characters_intention(LEVEL *level);

void remove_collision(LEVEL *level);

void update_characters(LEVEL *level);

#endif