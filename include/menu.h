#ifndef MENU_H
#define MENU_H

#include <game.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


#define SALVA_JOGO   1
#define CARREGA_JOGO 2
#define NOVO_JOGO    3
#define SAIR         0

int run_menu(ALLEGRO_FONT *font, ALLEGRO_EVENT event);



#endif // MENU_H
