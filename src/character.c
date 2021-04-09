#include <character.h>
#include <geometry.h>

void set_anim(ANIMATION *new_anim, CHARACTER *character){
    if(strcmp(character->current.name, new_anim->name)){
        character->current = *new_anim;
    }
}

void move_character(CHARACTER *character){
    if(module(character->direction) > 0.9) {
        set_anim(get_anim(character->anims, "_run"), character);
        if(character->direction.x < 0) character->flags |= ALLEGRO_FLIP_HORIZONTAL;
        else if(character->direction.x > 0) character->flags &= ~ALLEGRO_FLIP_HORIZONTAL;
        add_vector(&character->hitbox.center, character->direction, character->speed); 
    }
    else set_anim(get_anim(character->anims, "_idle"), character);
}

void update_character(CHARACTER *character, KEYBOARD_STATE kb_input, COORD pos_main){
    if(character->alive){
        switch(character->type){
            case MainCharacter:
                character->direction = input_to_vector(kb_input);
                move_character(character);
                break;
            case EnemySkeleton:
                character->direction = direction_from_to(character->hitbox.center, pos_main);
                move_character(character);
                break;
            case EnemyOgre:
                character->direction = direction_from_to(character->hitbox.center, pos_main);
                move_character(character);
                break;
            case EnemySpike:
                break;
        }
        al_draw_bitmap(animate(&character->current), 
                       character->pos_graphic.x + character->hitbox.center.x, 
                       character->pos_graphic.y + character->hitbox.center.y, 
                       character->flags);
    }
}


void set_character_hitbox(CHARACTER *character){
    character->hitbox.iscircle = 1;
    character->hitbox.radius = al_get_bitmap_width(character->current.frames[0].bitmap)/2;
    character->pos_graphic.x = -character->hitbox.radius;
    character->pos_graphic.y = -al_get_bitmap_height(character->current.frames[0].bitmap) + 
                               character->hitbox.radius + HITBOX_MARGIN_PX;
}