#include <input.h>

void set_kb_state(KEYBOARD_STATE *state, ALLEGRO_EVENT event){
    if(event.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(event.keyboard.keycode){
                case ALLEGRO_KEY_UP:
                    *state |= ARROWUP;
                    break;
                case ALLEGRO_KEY_DOWN:
                    *state |= ARROWDOWN;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    *state |= ARROWRIGHT;
                    break;
                case ALLEGRO_KEY_LEFT:
                    *state |= ARROWLEFT;
                    break;
            }
        }
    else if(event.type == ALLEGRO_EVENT_KEY_UP){
        if(event.keyboard.keycode == ALLEGRO_KEY_UP) *state &= ~ARROWUP;
        else if(event.keyboard.keycode == ALLEGRO_KEY_DOWN) *state &= ~ARROWDOWN;
        else if(event.keyboard.keycode == ALLEGRO_KEY_LEFT) *state &= ~ARROWLEFT;
        else if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT) *state &= ~ARROWRIGHT;
    }
}