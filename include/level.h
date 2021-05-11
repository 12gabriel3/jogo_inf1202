#ifndef LEVEL_H
#define LEVEL_H

#include <env.h>
#include <geometry.h>
#include <character.h>
#define ENVS_MAX 2000
#define CHARACTERS_MAX 1000
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
    AURA aura;
    KEYBOARD_STATE input;
} LEVEL;

void update_ui(LEVEL *level);

void update_characters(LEVEL *level);

void get_main_collision(LEVEL *level);

void atk(LEVEL *level);

void Coordenada(int linha,int coluna,float *cord_x,float *cord_y, char busca, char Mapa[]);

void Busca(char nome_arquivo_inp[MAX_NOME], FILE *arq, char mapa[][COLUNA], LEVEL *level1, ANIMATION *animacao, SPRITE *sprite);

void load_jogo(char Nome_Mapa[], char Mapa1 [][COLUNA], LEVEL *level,ANIMATION *anims1,SPRITE *sprite1);

int Salva_Jogo(char mapa[][COLUNA],char nome_arquivo_out[MAX_NOME],LEVEL *level1);

void atualiza_env(LEVEL *level);

void add_line(LEVEL *level);

#endif
