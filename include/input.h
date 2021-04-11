#ifndef INPUT_H
#define INPUT_H

#include <allegro5/allegro.h>


#define ARROWUP (1 << 0)
#define ARROWDOWN (1 << 1)
#define ARROWLEFT (1 << 2)
#define ARROWRIGHT (1 << 3)

typedef char KEYBOARD_STATE;

/**
 * @brief Atualiza o estado atual do teclado de acordo com o evento
 * 
 * @param state Representa quais teclas estao pressionadas
 * @param event O evento que será verificado
 */
void set_kb_state(KEYBOARD_STATE *state, ALLEGRO_EVENT event);

#endif