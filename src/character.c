#include <character.h>
#include <geometry.h>

void set_anim(ANIMATION *new_anim, CHARACTER *character){
    if(strcmp(character->current.name, new_anim->name)){
        character->current = *new_anim;
    }
}

void move_character(CHARACTER *character){
    float mod = module(character->direction);
    normalize(&character->direction);
    if(mod > 0.1) {
        set_anim(get_anim(character->anims, "_run"), character);
        if(character->direction.x < 0) {
            character->flags |= ALLEGRO_FLIP_HORIZONTAL;
        }
        else if(character->direction.x > 0) {
            character->flags &= ~ALLEGRO_FLIP_HORIZONTAL;
        }
        add_vector(&character->hitbox.center, character->direction, character->speed * mod); 
    }
    else set_anim(get_anim(character->anims, "_idle"), character);
}

void update_character(CHARACTER *character, KEYBOARD_STATE kb_input, CHARACTER *characters){
    if(character->alive){
        HITBOX next_pos = character->hitbox;
        int i;
        switch(character->type){
            case MainCharacter:
                character->direction = input_to_vector(kb_input);
                break;
            case EnemySkeleton:
                character->direction = direction_from_to(character->hitbox.center, characters[0].hitbox.center);
                break;
            case EnemyOgre:
                character->direction = direction_from_to(character->hitbox.center, characters[0].hitbox.center);
                break;
            case EnemySpike:
                break;
        }
        vec_sum(&next_pos.center, multiply(character->speed, character->direction));
        for(i = 0; i < 3; i++){
            if(character != &characters[i]){
                if(cc_collides(next_pos, characters[i].hitbox))
                    rm_direction(cc_colision_normal(characters[i].hitbox, next_pos), &character->direction);
            }
            
        }
        
        move_character(character);
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