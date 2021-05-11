#include <game.h>

void run_game(GAME *game, ALLEGRO_EVENT event){
    set_kb_state(&game->current_level.input, event);
    //limpa a tela p preto (tabela RGB 000) p cada atualiza��o

    if(event.type == ALLEGRO_EVENT_TIMER){
        al_clear_to_color(al_map_rgb_f(0, 0, 0));
        atualiza_env(&game->current_level);

        //atualiza as caracteristicas de cada char
        update_characters(&game->current_level);
        get_main_collision(&game->current_level);
        game->score += atk(&game->current_level);
        update_ui(&game->current_level, game->font, game->score);
        //desenha a linha vermelha p allegro
        // al_draw_line(0, 400, 600, 500, al_color_name("red"), 1);



        //atualiza tela (?) veio de um exemplo
        al_flip_display();
    }
}