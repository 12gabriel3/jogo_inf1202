#include <input.h>

void set_kb_state(KEYBOARD_STATE *state, ALLEGRO_EVENT event){
    if(event.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(event.keyboard.keycode){
                case ALLEGRO_KEY_UP:
                    *state |= UP;
                    break;
                case ALLEGRO_KEY_DOWN:
                    *state |= DOWN;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    *state |= RIGHT;
                    break;
                case ALLEGRO_KEY_LEFT:
                    *state |= LEFT;
                    break;
            }
        }
    else if(event.type == ALLEGRO_EVENT_KEY_UP){
        if(event.keyboard.keycode == ALLEGRO_KEY_UP) *state &= ~UP;
        else if(event.keyboard.keycode == ALLEGRO_KEY_DOWN) *state &= ~DOWN;
        else if(event.keyboard.keycode == ALLEGRO_KEY_LEFT) *state &= ~LEFT;
        else if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT) *state &= ~RIGHT;
    }
}