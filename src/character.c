#include <character.h>

void move_character(CHARACTER *character){
    if(character->direction.x || character->direction.y) character->current = get_anim(character->anims, "_run");
    else character->current = get_anim(character->anims, "_idle");
    if(character->direction.x < 0) character->flags |= ALLEGRO_FLIP_HORIZONTAL;
    if(character->direction.x > 0) character->flags &= ~ALLEGRO_FLIP_HORIZONTAL;
    add_vector(&character->pos, character->direction, character->speed);
}

void update_character(CHARACTER *character, KEYBOARD_STATE kb_input, COORD pos_main){
    if(character->alive){
        switch(character->type){
            case MainCharacter:
                character->direction = input_to_vector(kb_input);
                move_character(character);
                break;
            case EnemySkeleton:
                character->direction = direction_from_to(character->pos, pos_main);
                move_character(character);
                break;
            case EnemyOgre:
                character->direction = direction_from_to(character->pos, pos_main);
                move_character(character);
                break;
            case EnemySpike:
                break;
        }
        al_draw_bitmap(animate(character->current), character->pos.x, character->pos.y, character->flags);
    }
}
