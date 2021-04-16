// Autor: Gabriel Aguiar

#include <graphics.h>


char *lst_tok(char *string, char *separator){
    char *last_tok = NULL;
    char *tok;
    tok = strtok(string, separator);
    while(tok){
        last_tok = tok;
        tok = strtok(NULL, separator);
    }
    return last_tok;
}

void carrega_sprites(SPRITE *sprites, ANIMATION *animations, char *folder){
    ALLEGRO_FS_ENTRY *dir = al_create_fs_entry(folder);
    ALLEGRO_FS_ENTRY *file_entry;
    int n_animations = 0, n_sprites = 0, anim_frame = 0;
    char *file_name, sprite_name[SPRITE_NAME_SIZE + 1], animation_name[SPRITE_NAME_SIZE + 1];
    const char *file_path;
    char file_path_cp[FILE_PATH_SIZE + 1];
    al_open_directory(dir);
    file_entry = al_read_directory(dir);
    while(file_entry && n_sprites < SPRITES_MAX && n_animations < ANIMS_MAX){
        file_path = al_get_fs_entry_name(file_entry);
        printf("Lendo arquivo %s\n\n", file_path);
        strcpy(file_path_cp, file_path);
        printf("Chamando lst_tok");
        file_name = lst_tok(file_path_cp, "\\");
        // Substitui o ponto por '\0'
        *strrchr(file_name, '.') = '\0';
        strcpy(sprite_name, file_name);

        // Se o nome do sprite tiver "_f0" é o primeiro frame de uma animação, então:
        //      1. Registra o nome da animação
        //      2. Registra os frames
        //      3. Registra o numero de frames
        //      4. Avança o número de animações carregadas
        if(strstr(sprite_name, "_f0")){
            printf("Animacao encontrada!\n");
            // Registra o nome da animação
            strcpy(animation_name, sprite_name);
            *strrchr(animation_name, '_') = '\0';
            strcpy(animations[n_animations].name, animation_name);
            animations[n_animations].current_period = 0;
            animations[n_animations].play = 1;
            animations[n_animations].loop = 1;
            animations[n_animations].period = FPS/2;
            printf("Carregando animacao %s...\n", animation_name);
            anim_frame = 0;
            // Enquanto for parte da animação:
            //     1. Registra o nome do frame
            //     2. Carrega o bitmap
            //     3. Libera os recursos
            //     4. Le o próximo arquivo
            do{
                strcpy(animations[n_animations].frames[anim_frame].name, sprite_name);
                printf("Carregando sprite %s...\n", sprite_name);
                animations[n_animations].frames[anim_frame].bitmap = al_load_bitmap(file_path);

                al_destroy_fs_entry(file_entry);

                file_entry = al_read_directory(dir);
                if(file_entry){
                    file_path = al_get_fs_entry_name(file_entry);
                    strcpy(file_path_cp, file_path);
                    file_name = lst_tok(file_path_cp, "\\");
                    // Substitui o ponto por '\0'
                    *strrchr(file_name, '.') = '\0';
                    strcpy(sprite_name, file_name);
                    anim_frame++;
                }
            }while(strstr(sprite_name, animation_name) && file_entry);
            // Terminado de ler a animação, registra o número de frames e incrementa o numero de animações lidas
            animations[n_animations].n_frames = anim_frame;
            n_animations++;
        } else {
            printf("Sprite encontrado!\n");
            strcpy(sprites[n_sprites].name, sprite_name);
            sprites[n_sprites].bitmap = al_load_bitmap(file_path);
            printf("Carregando sprite %s...\n", sprite_name);
            al_destroy_fs_entry(file_entry);
            file_entry = al_read_directory(dir);
            n_sprites++;
        }
    }
    al_destroy_fs_entry(dir);
}

SPRITE *get_sprite(SPRITE *sprites, char *name){
    int i = 0;
    SPRITE *spr = NULL;
    while(!strstr(sprites[i].name, name)) i++;
    if(i != SPRITES_MAX) spr = sprites + i;
    return spr;
}

ANIMATION *get_anim(ANIMATION *anims, char *name){
    int i = 0;
    ANIMATION *anim = NULL;
    while(!strstr(anims[i].name, name)) i++;
    if(i != ANIMS_MAX) anim = anims + i;
    return anim;
}

ALLEGRO_BITMAP *animate(ANIMATION *anim){
    int current_frame;
    current_frame = (int) (anim->current_period / (float) anim->period * anim->n_frames);
    if(anim->play){
        if(anim->loop){
            anim->current_period = (anim->current_period + 1) % anim->period;
        } else {
            if(anim->current_period < anim->period - 1) anim->current_period++;
        }
    }
    
    return anim->frames[current_frame].bitmap;
}
