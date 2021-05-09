//define os tipos e constantes
//todas as contantes est�o no include, .h
#include <input.h>

void set_kb_state(KEYBOARD_STATE *state, ALLEGRO_EVENT event){
    if(event.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(event.keyboard.keycode){
                case ALLEGRO_KEY_UP:
                case ALLEGRO_KEY_W:
                    *state |= ARROWUP;
                    break;
                case ALLEGRO_KEY_DOWN:
                case ALLEGRO_KEY_S:
                    *state |= ARROWDOWN;
                    break;
                case ALLEGRO_KEY_RIGHT:
                case ALLEGRO_KEY_D:
                    *state |= ARROWRIGHT;
                    break;
                case ALLEGRO_KEY_LEFT:
                case ALLEGRO_KEY_A:
                    *state |= ARROWLEFT;
                    break;
            }
        }
    else if(event.type == ALLEGRO_EVENT_KEY_UP){
        switch(event.keyboard.keycode){
                case ALLEGRO_KEY_UP:
                case ALLEGRO_KEY_W:
                    *state &= ~ARROWUP;
                    break;
                case ALLEGRO_KEY_DOWN:
                case ALLEGRO_KEY_S:
                    *state &= ~ARROWDOWN;
                    break;
                case ALLEGRO_KEY_RIGHT:
                case ALLEGRO_KEY_D:
                    *state &= ~ARROWRIGHT;
                    break;
                case ALLEGRO_KEY_LEFT:
                case ALLEGRO_KEY_A:
                    *state &= ~ARROWLEFT;
                    break;
            }
    }
}
