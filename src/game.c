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
        switch(level_over(&game->current_level)){
            case KILLED_MONSTERS:
                al_draw_text(game->font, al_color_name("yellow"), 100, 100, 0, "Voce matou todos os monstros");
                break;
            case GOT_TREASURE:
                al_draw_text(game->font, al_color_name("yellow"), 100, 100, 0, "Voce pegou o testouro");
                break;
            case GAME_OVER:
                al_draw_text(game->font, al_color_name("yellow"), 100, 100, 0, "GAME OVER");
                break;

        }

        //atualiza tela (?) veio de um exemplo
        al_flip_display();
    }
}