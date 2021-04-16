#include <character.h>
#include <geometry.h>

<<<<<<< HEAD
void move_character(CHARACTER *character){
    if(character->direction.x || character->direction.y) character->current = *get_anim(character->anims, "_run");
    else character->current = *get_anim(character->anims, "_idle");
    if(character->direction.x < 0) character->flags |= ALLEGRO_FLIP_HORIZONTAL;
    if(character->direction.x > 0) character->flags &= ~ALLEGRO_FLIP_HORIZONTAL;
    add_vector(&character->pos, character->direction, character->speed);
=======
void set_anim(ANIMATION *new_anim, CHARACTER *character){
    if(strcmp(character->current.name, new_anim->name)){
        character->current = *new_anim;
    }
>>>>>>> fc5b3670360a24cf3a73add142f5ed8d9e67dd54
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
<<<<<<< HEAD
        al_draw_bitmap(animate(&character->current), character->pos.x, character->pos.y, character->flags);
=======
>>>>>>> fc5b3670360a24cf3a73add142f5ed8d9e67dd54
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