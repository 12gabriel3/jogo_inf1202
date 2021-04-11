#ifndef LEVEL_H
#define LEVEL_H

#include <env.h>
#include <geometry.h>
#include <character.h>

#define ENVS_MAX 200
#define ENEMIES_MAX 50
#define LINES_MAX 100

typedef struct {
    ENV envs[ENVS_MAX];
    int n_envs;
    CHARACTER main_character;
    CHARACTER enemies[ENEMIES_MAX];
    int n_enemies;
    LINE lines[LINES_MAX];
    int n_lines;
} LEVEL;

#endif