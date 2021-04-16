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
        al_draw_bitmap(animate(&character->current), character->hitbox.bounds.center.x + character->pos_graphic.x, 
                       character->hitbox.bounds.center.y + character->pos_graphic.y, character->flags);
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