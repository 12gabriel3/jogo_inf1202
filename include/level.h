#ifndef LEVEL_H
#define LEVEL_H

#include <env.h>
#include <geometry.h>
#include <character.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_color.h>
#include <stdlib.h>
#include <time.h>

#define ENVS_MAX 2000
#define CHARACTERS_MAX 200
#define LINES_MAX 2000
#define UI_MAX 50

#define GO_ON 0
#define GAME_OVER 1
#define KILLED_MONSTERS 2
#define GOT_TREASURE 3

typedef struct {
    SPRITE *heart_full, *heart_empty;
    ENV envs[ENVS_MAX];
    CIRCLE chest;
    int n_envs;
    CHARACTER characters[CHARACTERS_MAX];
    int n_characters;
    LINE lines[LINES_MAX];
    int n_lines;
    AURA aura;
    KEYBOARD_STATE input;
} LEVEL;

void update_ui(LEVEL *level, ALLEGRO_FONT *font, int score);

void update_characters(LEVEL *level);

void get_main_collision(LEVEL *level);

int atk(LEVEL *level, ANIMATION *anims);

void Coordenada(int linha,int coluna,float *cord_x,float *cord_y, char busca, char Mapa[]);

int Busca(char nome_arquivo_inp[MAX_NOME], FILE *arq, LEVEL *level1, ANIMATION *animacao, SPRITE *sprite);

int load_mapa(char Nome_Mapa[], LEVEL *level,ANIMATION *anims1,SPRITE *sprite1);

void atualiza_env(LEVEL *level);

void add_line(LEVEL *level);

int level_over(LEVEL *level);

#endif
