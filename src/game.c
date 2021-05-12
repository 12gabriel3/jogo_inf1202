#include <game.h>


int run_game(GAME *game, ALLEGRO_EVENT event)
{
    if(game->state == PAUSE)
    {
        switch (run_menu(game->font,event))
        {
        case SAIR:
            return 0;
            break;
        case SALVA_JOGO:
            Salva_Jogo("../save_game/game.save",game);
            break;
        case CARREGA_JOGO:
            load_jogo("../save_game/game.save",game);
            break;
        case NOVO_JOGO:
            load_mapa("../Map/FASE 1.txt",&game->current_level,game->anims,game->sprites);
            game->state = PLAY;
            break;
        case CONTINUA:
            game->state = PLAY;
            break;
        }

    }
    else
    {
        set_kb_state(&game->current_level.input, event);
        //limpa a tela p preto (tabela RGB 000) p cada atualiza��o
        if(game->current_level.input &ESC)
        {
            game->current_level.input &= ~ESC;
            game->state = PAUSE;
        }

        else if(event.type == ALLEGRO_EVENT_TIMER)
        {
            al_clear_to_color(al_map_rgb_f(0, 0, 0));
            atualiza_env(&game->current_level);

            //atualiza as caracteristicas de cada char
            update_characters(&game->current_level);
            get_main_collision(&game->current_level);
            game->score += atk(&game->current_level);
            update_ui(&game->current_level, game->font, game->score);
            switch(level_over(&game->current_level))
            {
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
    return 1;
}


int Salva_Jogo(char nome_arquivo_out[MAX_NOME],GAME *game)
{
    FILE *arq;
    int i,falha = 0;
    if((arq = fopen((nome_arquivo_out),"wb+")))
    {
        fwrite(game,sizeof(*game),1,arq);
    }
    else
        falha = 1;

    fclose(arq);

    return falha;
}

int load_jogo(char nome_arquivo_out[MAX_NOME],GAME *game)
{
    FILE *arq;
    int i,falha = 0;

    if((arq = fopen((nome_arquivo_out),"rb")))
    {
        fread(game,sizeof(*game),1,arq);
    }
    else
        falha = 1;

    fclose(arq);

    carrega_sprites(game->sprites, game->anims, "../img"); //.. :Diretorio de cima

    game->state = PLAY;
    game->current_level.aura.anim = *get_anim(game->anims, "aura");
    game->font = al_load_font("../fonts/PressStart2P-Regular.ttf", -20, 0);
    game->current_level.heart_full = get_sprite(game->sprites, "ui_heart_full");
    game->current_level.heart_empty = get_sprite(game->sprites, "ui_heart_empty");

    game->current_level.characters[0].anims = get_anim(game->anims,"wizzard_f");
    game->current_level.characters[0].current = game->current_level.characters[0].anims[0];

    for(i=1; i < game->current_level.n_characters; i++)
    {
        switch (game->current_level.characters[i].type)
        {
        case EnemyOgre:
            game->current_level.characters[i].anims = get_anim(game->anims,"big_zombie");
            game->current_level.characters[i].current = game->current_level.characters[i].anims[0];
            break;
        case EnemySkeleton:
            game->current_level.characters[i].anims = get_anim(game->anims,"skelet_idle");
            game->current_level.characters[i].current = game->current_level.characters[i].anims[0];
            break;
        case EnemySpike:
            game->current_level.characters[i].anims = get_anim(game->anims,"floor_spikes");
            game->current_level.characters[i].current = game->current_level.characters[i].anims[0];
        }
    }

    for(i=0; i < game->current_level.n_envs; i++)
    {
        if(game->current_level.envs[i].is_anim) game->current_level.envs[i].anim = *get_anim(game->anims, game->current_level.envs[i].anim.name);
        else game->current_level.envs[i].sprite = get_sprite(game->sprites, game->current_level.envs[i].sprite->name);
    }

    return falha;
}
