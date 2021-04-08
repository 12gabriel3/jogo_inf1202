#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <string.h>


#define FPS 60

#define ANIMS_MAX 500
#define SPRITES_MAX 500

#define SPRITE_NAME_SIZE 35
#define ANIMATION_SIZE 8
#define ANIMATION_NAME_SIZE 35
#define FILE_NAME_SIZE 40
#define FRAME_ID_SIZE 3
#define FILE_PATH_SIZE 100
#define FILE_EXTENSION_SIZE 3

/**
 * @brief Uma imagem com nome
 * 
 */
typedef struct 
{
    char name[SPRITE_NAME_SIZE + 1];
    ALLEGRO_BITMAP *bitmap;
} SPRITE;

/**
 * @brief Uma sequencia de frames, com informações sobre como ela deve ser executada
 * 
 */
typedef struct {
    char name[ANIMATION_NAME_SIZE + 1];
    SPRITE frames[ANIMATION_SIZE];
    int n_frames, current_period, period, loop, play;
} ANIMATION;

/**
 * @brief Retorna o ultimo token de uma string
 * 
 * @param string a string a ser tokenizada (ela é modificada)
 * @param separator o separador usado para tokenizar
 * @return char* aponta para a primeira posição do ultimo token
 */
char *lst_tok(char *string, char *separator);

/**
 * @brief Carrega as sprites e animações do disco
 * 
 * @param sprites onde as sprites são gravadas
 * @param animations onde as animações sao gravadas
 * @param folder o diretório onde estao as imagens
 */
void carrega_sprites(SPRITE *sprites, ANIMATION *animations, char *folder);

/**
 * @brief Retorna a primeira sprite de uma lista de sprites que contenha name no atributo name
 * 
 * @param sprites Uma lista de sprites que será percorrida
 * @param name A string que o campo nome da sprite deve conter
 * @return SPRITE* Aponta para a primeira sprite cujo atributo name contem a string name
 */
SPRITE *get_sprite(SPRITE *sprites, char *name);

/**
 * @brief Retorna a primeira animação de uma lista de animações que contenha name no atributo name
 * 
 * @param anims Uma lista de animações que será percorrida
 * @param name A string que o campo nome da animação deve conter
 * @return ANIMATION* Aponta para a primeira animação cujo atributo name contem a string name
 */
ANIMATION *get_anim(ANIMATION *anims, char *name);

/**
 * @brief Retorna a imagem do frame atual de uma animação e avança seu estado
 * 
 * @param anim a animação que será animda. Seu estado é modificado
 * @return ALLEGRO_BITMAP* A imagem do frame atual da animação
 */
ALLEGRO_BITMAP *animate(ANIMATION *anim);


#endif