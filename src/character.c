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
    }
    else set_anim(get_anim(character->anims, "_idle"), character);
    add_vector(&character->hitbox.bounds.center, character->direction, character->speed); 
}

void set_character_hitbox(CHARACTER *character){
    character->hitbox.r = al_get_bitmap_width(character->current.frames[0].bitmap)/2;
    set_circle_box(&character->hitbox);
    character->pos_graphic.x = -character->hitbox.r;
    character->pos_graphic.y = -al_get_bitmap_height(character->current.frames[0].bitmap) + 
                               character->hitbox.r + HITBOX_MARGIN_PX;
}

void set_atk_hitbox(AURA *aura){
    int current_frame;
    current_frame = (int) (aura->anim.current_period / (float) aura->anim.period * aura->anim.n_frames);
    aura->hitbox.r = al_get_bitmap_width(aura->anim.frames[current_frame].bitmap)/2;
    set_circle_box(&aura->hitbox);
    aura->pos_graphic.x = -aura->hitbox.r;
    aura->pos_graphic.y = -al_get_bitmap_height(aura->anim.frames[0].bitmap) + 
                               aura->hitbox.r + HITBOX_MARGIN_PX;
}