#include <level.h>

void set_characters_intention(LEVEL *level)
{
    int i;
    for(i = 0; i < level->n_characters; i++)
    {
        switch(level->characters[i].type)
        {
        case MainCharacter:
            level->characters[i].direction = input_to_vector(level->input);
            break;
        case EnemySkeleton:
            level->characters[i].direction = direction_from_to(level->characters[i].hitbox.bounds.center, level->characters[0].hitbox.bounds.center);
            if(module(dist_from_to(level->characters[i].hitbox.bounds.center, level->characters[0].hitbox.bounds.center)) < 200)
                level->characters[i].speed = 2.7;
            else
                level->characters[i].speed = 1;
            break;
        case EnemyOgre:
            level->characters[i].direction = direction_from_to(level->characters[i].hitbox.bounds.center, level->characters[0].hitbox.bounds.center);
            break;
        case EnemySpike:
            break;
        }
    }

}

//remove as colisoes
int remove_collision(LEVEL *level)
{
    COORD normal;
    CIRCLE next_pos_i, next_pos_j;
    int i, j, k, collision;
    collision = 0;
    for(i = 0; i < level->n_characters; i++)
    {
        if(level->characters[i].lives > 0)
        {
            for(j = 0; j < level->n_characters; j++)
            {
                if(level->characters[j].lives > 0)
                {
                    //se tiver uma colisao ele tira a velocidade do char
                    if(&level->characters[j] != &level->characters[i])
                    {
                        next_pos_i = level->characters[i].hitbox;
                        vec_sum(&next_pos_i.bounds.center, multiply(level->characters[i].speed, level->characters[i].direction));
                        next_pos_j = level->characters[j].hitbox;
                        vec_sum(&next_pos_j.bounds.center, multiply(level->characters[j].speed, level->characters[j].direction));
                        normal = cc_collision_normal(next_pos_j, next_pos_i);
                        if(dot_prod(normal, level->characters[i].direction) > 0)
                        {
                            normal = multiply(-0.5, normal);
                            vec_sum(&level->characters[i].direction, normal);
                            collision = 1;
                        }
                        if(module(level->characters[i].direction) > 1)
                            normalize(&level->characters[i].direction);
                    }
                    //remove as colisoes com os muros
                    for(k = 0; k < level->n_lines; k++)
                    {
                        next_pos_i = level->characters[i].hitbox;
                        vec_sum(&next_pos_i.bounds.center, multiply(level->characters[i].speed, level->characters[i].direction));
                        normal = lc_collision_normal(next_pos_i, level->lines[k]);
                        if(dot_prod(normal, level->characters[i].direction) > 0)
                        {
                            vec_sum(&level->characters[i].direction, multiply(-0.5, normal));
                            collision = 1;
                        }
                        if(module(level->characters[i].direction) > 1)
                            normalize(&level->characters[i].direction);
                    }
                }
            }
        }

    }
    return collision;
}

void get_main_collision(LEVEL *level)
{
    int i;
    i = 1;
    if(level->characters[0].inv_timer == 0)
    {
        while(!cc_collides(level->characters[0].hitbox, level->characters[i].hitbox) && i < level->n_characters)
            i++;
        if(i < level->n_characters)
        {
            level->characters[0].lives--;
            level->characters[0].inv_timer = INV_TIMER;
        }
    }
    else
    {
        level->characters[0].inv_timer--;
    }

}

void update_characters(LEVEL *level)
{
    int i;
    set_characters_intention(level);
    remove_collision(level);
    for(i = 0; i < level->n_characters; i++)
    {
        if(level->characters[i].lives > 0)
        {
            move_character(&level->characters[i]);
            al_draw_bitmap(animate(&level->characters[i].current),
                           level->characters[i].pos_graphic.x + level->characters[i].hitbox.bounds.center.x,
                           level->characters[i].pos_graphic.y + level->characters[i].hitbox.bounds.center.y,
                           level->characters[i].flags);
        }
    }
}

void atk(LEVEL *level){
    if(level->input | ATTACK && level->characters[0].atk_timer == 0){
        level->characters[0].atk_timer = ATTACK_TIMER;
        level->aura.hitbox.bounds.center = level->characters[0].hitbox.bounds.center;
        set_atk_hitbox(&level->aura);
        al_draw_bitmap(animate(&level->aura.anim), 
                       level->aura.hitbox.bounds.center.x + level->aura.pos_graphic.x, 
                       level->aura.hitbox.bounds.center.y + level->aura.pos_graphic.y, 0);
    }
}

void update_ui(LEVEL *level)
{
    int i;
    for(i = 0; i < 3; i++)
    {
        if(i < level->characters[0].lives)
            al_draw_bitmap(level->heart_full->bitmap, i*16, 0, 0);
        else
            al_draw_bitmap(level->heart_empty->bitmap, i*16, 0, 0);
    }
}




void load_jogo(char Nome_Mapa[], char Mapa1 [][COLUNA], LEVEL *level,ANIMATION *anims1)
{
    FILE *Arq;

    Busca(Nome_Mapa, Arq, Mapa1, level, anims1);
}



int Salva_Jogo(char mapa[][COLUNA],char nome_arquivo_out[MAX_NOME])
{
    FILE *arq;
    int i,falha = 0;
    if((arq = fopen((nome_arquivo_out),"w+")))
     {
         for(i=0; i<LINHA; i++)
             fputs(mapa[i],arq);

         if(fprintf(arq,"\n\n--------------------------------------------------------------\n")< 0)
             falha = 1;
        /*  if(fprintf(arq,"\nFase: %d\n",obj->Fase) < 0)
             falha = 1;
         if(fprintf(arq,"\nVidas: %d\n",obj->Vida) < 0)
             falha = 1;
         if(fprintf(arq,"\nPontos: %d\n",obj->Pontos) < 0)
             falha = 1;
        */
     }
     else
    falha = 1;

    if(!falha)
        return 1;
    else
        return 0;
    fclose(arq);
}

void Busca(char nome_arquivo_inp[MAX_NOME], FILE *arq, char mapa[][COLUNA], LEVEL *level1, ANIMATION *animacao)
{
    int i,j,n_monstro = 0;
    char texto_str[COLUNA];

    if(!(arq = fopen((nome_arquivo_inp),"r")))
    {
        printf("\n\n Erro na abertura do arquivo de leitura! \n\n");              // Escreve mensagem de erro caso n�o consiga executar a funcao
    }
    else
    {
        do
        {
            while(fgets(texto_str, COLUNA, arq) != NULL)
            {
                strncpy(mapa[i],texto_str,COLUNA);
                i++;
            }
        }
        while(!feof(arq));
        fclose(arq);

        for(i=0; i<LINHA; i++)
        {
            Coordenada(i,0, &(level1->characters[(level1->n_characters)].hitbox.bounds.center.x), &(level1->characters[(level1->n_characters)].hitbox.bounds.center.y), 'J', mapa[i]);
            if(((level1->characters[(level1->n_characters)].hitbox.bounds.center.x)!= 0)||((level1->characters[(level1->n_characters)].hitbox.bounds.center.y) != 0))                                  //verifica se a posi��o atual da matriz ainda � zero, caso contrario atualiza o numero de OGROS no mapa
            {
                level1->characters[(level1->n_characters)].anims = get_anim(animacao, "wizzard_m");
                level1->characters[(level1->n_characters)].current = level1->characters[(level1->n_characters)].anims[0];
                level1->characters[(level1->n_characters)].speed = 3;
                set_character_hitbox(&level1->characters[(level1->n_characters)]);
                level1->characters[(level1->n_characters)].lives = 3;
                level1->characters[(level1->n_characters)].type = MainCharacter;
                level1->characters[(level1->n_characters)].inv_timer = 0;
                (level1->n_characters) += 1;
            }
            /*
            Coordenada(i,0, &(level1->characters[(level1->n_characters)].pos_graphic.x), &(level1->characters[(level1->n_characters)].pos_graphic.y), 'T', mapa[i]);
            if(((level1->characters[(level1->n_characters)].pos_graphic.x)!= 0)||((level1->characters[(level1->n_characters)].pos_graphic.y) != 0))                                 //verifica se a posi��o atual da matriz ainda � zero, caso contrario atualiza o numero de OGROS no mapa
            {
                level1->characters[(level1->n_characters)].anims = get_anim(animacao, "big_zombie");
                level1->characters[(level1->n_characters)].current = level1->characters[(level1->n_characters)].anims[0];
                level1->characters[(level1->n_characters)].speed = 2;
                set_character_hitbox(&level1->characters[i]);
                level1->characters[(level1->n_characters)].lives = 1;
                level1->characters[(level1->n_characters)].type = EnemyOgre;
                level1->characters[(level1->n_characters)].hitbox.bounds.center.x = 300;
                level1->characters[(level1->n_characters)].hitbox.bounds.center.y = i * 50 - 900;
                (level1->n_characters) += 1;
            }

            Coordenada(i,0, &(level1->characters[(level1->n_characters)].pos_graphic.x), &(level1->characters[(level1->n_characters)].pos_graphic.y), 'Z', mapa[i]);
            if(((level1->characters[(level1->n_characters)].pos_graphic.x)!= 0)||((level1->characters[(level1->n_characters)].pos_graphic.y) != 0))                                 //verifica se a posi��o atual da matriz ainda � zero, caso contrario atualiza o numero de OGROS no mapa
            {
                level1->characters[(level1->n_characters)].anims = get_anim(animacao, "skelet_idle");
                level1->characters[(level1->n_characters)].current = level1->characters[(level1->n_characters)].anims[0];
                level1->characters[(level1->n_characters)].speed = 1;
                set_character_hitbox(&level1->characters[(level1->n_characters)]);
                level1->characters[(level1->n_characters)].lives = 1;
                level1->characters[(level1->n_characters)].type = EnemySkeleton;
                //determinam a posicao do ps, seu centro x y
                level1->characters[(level1->n_characters)].hitbox.bounds.center.x = 600;
                level1->characters[(level1->n_characters)].hitbox.bounds.center.y = i * 50 - 100;
                (level1->n_characters) += 1;
            }



            for(j=0; j<COLUNA; j++)
            {
                Coordenada(i,j, &(level1->envs[(level1->n_envs)].pos_graphic.x), &(level1->envs[(level1->n_envs)].pos_graphic.y), '#', mapa[i]);
                if(((level1->envs[(level1->n_envs)].pos_graphic.x)!= 0)||((level1->envs[(level1->n_envs)].pos_graphic.y) != 0))                                  //verifica se a posi��o atual da matriz ainda � zero, caso contrario atualiza o numero de OGROS no mapa
                {
                    (level1->n_envs) += 1;
                }
            }

            for(j=0; j<COLUNA; j++)
            {
                Coordenada(i,j, &(level1->envs[(level1->n_envs)].pos_graphic.x), &(level1->envs[(level1->n_envs)].pos_graphic.y), 'X', mapa[i]);
                if(((level1->envs[(level1->n_envs)].pos_graphic.x)!= 0)||((level1->envs[(level1->n_envs)].pos_graphic.y) != 0))                                  //verifica se a posi��o atual da matriz ainda � zero, caso contrario atualiza o numero de OGROS no mapa
                {
                    (level1->n_envs) += 1;
                }
            }
            // Coordenada(i,0, &(teste->Bau_x), &(teste->Bau_y), 'B', mapa[i]);*/
        }
    }
}

void Coordenada(int linha,int coluna,float *cord_x,float *cord_y, char busca, char Mapa[])
{
    int corre_coluna;
    if((busca != '#') && (busca != 'X'))
    {
        for(corre_coluna = 0; corre_coluna < COLUNA; corre_coluna ++)
        {
            if(Mapa[corre_coluna] == busca)
            {

                *cord_y = corre_coluna;
                *cord_x = linha;
            }
        }
    }
    if((busca == '#') || (busca == 'X'))
    {
        if(Mapa[coluna] == busca)
        {
            *cord_y = coluna;
            *cord_x = linha;
        }
    }
}
