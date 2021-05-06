#ifndef ENV_H
#define ENV_H
#define LINHA 23
#define COLUNA 63
#define MAX_NOME 20
#define MAX_INIMIGOS 100
#define N_Max 1000
#define TROLL 1
#define ZUMBI 2

#include <geometry.h>
#include <graphics.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    ANIMATION *anim;
    SPRITE *sprite;
    COORD pos_graphic;
} ENV;

void update_env(ENV *col);


/*                                                     Mudança sem Gabriel
-----------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct Posicao
{
    int coordenada_x;
    int coordenada_y;

} POSICAO;

typedef struct Montros
{
    int coordenada_x[MAX_INIMIGOS];
    int coordenada_y[MAX_INIMIGOS];
    int Typo[MAX_INIMIGOS];
} MONSTROS;
typedef struct Tcc
{
    int Parede_x[N_Max];
    int Parede_y[N_Max];
    int N_Parede;
    int Espinho_x[N_Max];
    int Espinho_y[N_Max];
    int N_Espinho;
    int Bau_x;
    int Bau_y;
    int Pontos;
    int Fase;
    int Vida;
}OBJ;



void Coordenada(int linha,int coluna,int *cord_x,int *cord_y, char busca, char Mapa[]);

void Busca(char nome_arquivo_inp[MAX_NOME], FILE *arq, POSICAO *pos, MONSTROS *monstro, OBJ *teste, int *N_Troll, int *N_Zumbi, char mapa[][COLUNA]);

void Atualiza_jogo(MONSTROS *Monstro,POSICAO *Change, char Nome_Mapa[], char Mapa1 [][COLUNA], OBJ *Teste1);

int Salva_Jogo(char mapa[][COLUNA],char nome_arquivo_out[MAX_NOME],OBJ *obj);




#endif
