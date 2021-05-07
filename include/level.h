#ifndef LEVEL_H
#define LEVEL_H

#include <env.h>
#include <geometry.h>
#include <character.h>
#define ENVS_MAX 200
#define CHARACTERS_MAX 50
#define LINES_MAX 100
#define UI_MAX 50

typedef struct {
    SPRITE *heart_full, *heart_empty;
    ENV envs[ENVS_MAX];
    int n_envs;
    CHARACTER characters[CHARACTERS_MAX];
    int n_characters;
    LINE lines[LINES_MAX];
    int n_lines;
    KEYBOARD_STATE input;
} LEVEL;

void update_ui(LEVEL *level);

void update_characters(LEVEL *level);

void get_main_collision(LEVEL *level);

#endif