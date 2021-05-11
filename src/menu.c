#include <menu.h>

int run_menu(ALLEGRO_FONT *font, ALLEGRO_EVENT event)
{
    int valor = -1;
    if(event.type == ALLEGRO_EVENT_TIMER)
    {
        al_clear_to_color(al_map_rgb_f(65, 105, 225));
        al_draw_text(font, al_color_name("yellow"), 100, 100, 0, "NOVO JOGO (N)");
        al_draw_text(font, al_color_name("yellow"), 100, 130, 0, "CARREGAR JOGO (C)");
        al_draw_text(font, al_color_name("yellow"), 100, 160, 0, "SALVAR JOGO (S)");
        al_draw_text(font, al_color_name("yellow"), 100, 190, 0, "SAIR (Q)");
        al_flip_display();
    }
    if(event.type == ALLEGRO_EVENT_KEY_DOWN)
        switch(event.keyboard.keycode)
        {
        case ALLEGRO_KEY_N:
            valor = NOVO_JOGO;
            break;
        case ALLEGRO_KEY_C:
            valor = CARREGA_JOGO;
            break;
        case ALLEGRO_KEY_S:
            valor = SALVA_JOGO;
            break;
        case ALLEGRO_KEY_Q:
            valor = SAIR;
            break;
        case ALLEGRO_KEY_TAB:
            valor = CONTINUA;
            break;
        }
    return valor;
}



