#include <game.h>
#include <string.h>

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
            game->current_level.aura.anim.period = 20;
            game->current_level.input = 0;
            break;
        case NOVO_JOGO:
            game->current_level.characters[0].lives = 3;
            game->state = PLAY;
            game->current_level.aura.anim = *get_anim(game->anims, "aura");
            game->current_level.aura.anim.period = 20;
            game->current_level.aura.active = 0;
            game->current_level.n_characters = 1;
            game->current_level.n_envs = 0;
            game->current_level.n_lines = 0;
            game->n_level = 1;
            load_mapa("../Map/FASE 1.txt",&game->current_level,game->anims,game->sprites);
            game->score = 0;

            game->current_level.heart_full = get_sprite(game->sprites, "ui_heart_full");
            game->current_level.heart_empty = get_sprite(game->sprites, "ui_heart_empty");
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
            game->current_level.input = 0;;
            game->state = PAUSE;
        }

        else if(event.type == ALLEGRO_EVENT_TIMER)
        {
            al_clear_to_color(al_map_rgb_f(0, 0, 0));
            atualiza_env(&game->current_level);

            //atualiza as caracteristicas de cada char
            update_characters(&game->current_level);
            get_main_collision(&game->current_level);
            game->score += atk(&game->current_level, game->anims);
            update_ui(&game->current_level, game->font, game->score);
            switch(level_over(&game->current_level))
            {
            case KILLED_MONSTERS:
                next_level(game);
                break;
            case GOT_TREASURE:
                game->score += 100;
                next_level(game);
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

void next_level(GAME *game){
    char level[22] = "../Map/FASE ";
    char number[4];
    game->n_level++;
    strcat(level, itoa(game->n_level, number, 10));
    strcat(level, ".txt");
    game->current_level.aura.active = 0;
    game->current_level.n_characters = 1;
    game->current_level.n_envs = 0;
    game->current_level.n_lines = 0;
    if(load_mapa(level,&game->current_level,game->anims,game->sprites)) {
        game->n_level = 0;
        next_level(game);
    }
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
        game->current_level.characters[i].anims = get_anim(game->anims,game->current_level.characters[i].anims->name);
        game->current_level.characters[i].current = game->current_level.characters[i].anims[0];
    }

    for(i=0; i < game->current_level.n_envs; i++)
    {
        if(game->current_level.envs[i].is_anim) {
            game->current_level.envs[i].anim = *get_anim(game->anims, game->current_level.envs[i].anim.name);
            if(!strcmp(game->current_level.envs[i].anim.name, "chest_full_open_anim")) {
                game->current_level.envs[i].anim.loop = 0;
                game->current_level.envs[i].anim.play = 0;
            }
        }
        
        else game->current_level.envs[i].sprite = get_sprite(game->sprites, game->current_level.envs[i].sprite->name);
    }

    return falha;
}
