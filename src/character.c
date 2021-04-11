#include <character.h>
#include <geometry.h>

void set_anim(ANIMATION *new_anim, CHARACTER *character){
    if(strcmp(character->current.name, new_anim->name)){
        character->current = *new_anim;
    }
}

void move_character(CHARACTER *character){
    if(module(character->direction) * character->speed > 0.2) {
        set_anim(get_anim(character->anims, "_run"), character);
        if(character->direction.x < -0.1) {
            character->flags |= ALLEGRO_FLIP_HORIZONTAL;
        }
        else if(character->direction.x > 0.1) {
            character->flags &= ~ALLEGRO_FLIP_HORIZONTAL;
        }
        add_vector(&character->hitbox.bounds.center, character->direction, character->speed); 
    }
    else set_anim(get_anim(character->anims, "_idle"), character);
}

void update_character(CHARACTER *character, KEYBOARD_STATE kb_input, CHARACTER *characters, LINE l){
    if(character->alive){
        CIRCLE next_pos = character->hitbox;
        COORD normal;
        int i;
        switch(character->type){
            case MainCharacter:
                character->direction = input_to_vector(kb_input);
                break;
            case EnemySkeleton:
                character->direction = direction_from_to(character->hitbox.bounds.center, characters[0].hitbox.bounds.center);
                if(module(dist_from_to(character->hitbox.bounds.center, characters[0].hitbox.bounds.center)) < 200) character->speed = 2.7;
                else character->speed = 1;
                break;
            case EnemyOgre:
                character->direction = direction_from_to(character->hitbox.bounds.center, characters[0].hitbox.bounds.center);
                break;
            case EnemySpike:
                break;
        }
        vec_sum(&next_pos.bounds.center, multiply(character->speed, character->direction));
        for(i = 0; i < 20; i++){
            if(character != &characters[i]){
                normal = cc_collision_normal(characters[i].hitbox, next_pos);
                if(normal.x || normal.y)
                    rm_direction(normal, &character->direction);
            }
        }
        normal = lc_collision_normal(character->hitbox, l);
        if(normal.x || normal.y)
            rm_direction(normal, &character->direction);
        
        move_character(character);
        al_draw_bitmap(animate(&character->current), 
                       character->pos_graphic.x + character->hitbox.bounds.center.x, 
                       character->pos_graphic.y + character->hitbox.bounds.center.y, 
                       character->flags);
    }
}


void set_character_hitbox(CHARACTER *character){
    character->hitbox.r = al_get_bitmap_width(character->current.frames[0].bitmap)/2;
    set_circle_box(&character->hitbox);
    character->pos_graphic.x = -character->hitbox.r;
    character->pos_graphic.y = -al_get_bitmap_height(character->current.frames[0].bitmap) + 
                               character->hitbox.r + HITBOX_MARGIN_PX;
}