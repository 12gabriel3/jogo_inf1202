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
        case EnemyOgre:
            level->characters[i].direction = direction_from_to(level->characters[i].hitbox.bounds.center, level->characters[0].hitbox.bounds.center);
            if(module(dist_from_to(level->characters[i].hitbox.bounds.center, level->characters[0].hitbox.bounds.center)) < 150 && level->characters[i].lives == 1)
                level->characters[i].speed = 1.5;
            else
                level->characters[i].speed = 0.8;
            break;
        case EnemySkeleton:
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
        if(level->characters[i].lives > 0 && level->characters[i].type != EnemySpike)
        {
            for(j = 0; j < level->n_characters; j++)
            {
                if(level->characters[j].lives > 0 &&
                        !(level->characters[i].type == EnemySkeleton && level->characters[j].type == EnemySpike) &&
                        !(level->characters[i].type == MainCharacter && level->characters[j].type == EnemySpike) )
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
                }
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
    return collision;
}

void get_main_collision(LEVEL *level)
{
    int i;
    i = 1;
    if(level->characters[0].inv_timer == 0)
    {
        while((!level->characters[i].lives || !cc_collides(level->characters[0].hitbox, level->characters[i].hitbox)) && i < level->n_characters)
            i++;
        if(i < level->n_characters && level->characters[0].lives)
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
    for(i = level->n_characters - 1; i > -1; i--)
    {
        if(level->characters[i].type == EnemySpike)
        {
            al_draw_bitmap(animate(&level->characters[i].current),
                           level->characters[i].pos_graphic.x + level->characters[i].hitbox.bounds.center.x,
                           level->characters[i].pos_graphic.y + level->characters[i].hitbox.bounds.center.y,
                           level->characters[i].flags);
        }
    }
    for(i = level->n_characters - 1; i > -1; i--)
    {
        if(level->characters[i].lives > 0 && level->characters[i].type != EnemySpike)
        {
            move_character(&level->characters[i]);
            al_draw_bitmap(animate(&level->characters[i].current),
                           level->characters[i].pos_graphic.x + level->characters[i].hitbox.bounds.center.x,
                           level->characters[i].pos_graphic.y + level->characters[i].hitbox.bounds.center.y,
                           level->characters[i].flags);
        }
    }
}

int atk(LEVEL *level, ANIMATION *anims)
{
    int i, points = 0;
    if(level->input & ATTACK && level->characters[0].atk_timer == 0)
    {
        level->aura.active = 1;
        level->characters[0].atk_timer = ATTACK_TIMER;
    }
    if(level->aura.active)
    {
        level->aura.hitbox.bounds.center = level->characters[0].hitbox.bounds.center;
        set_atk_hitbox(&level->aura);
        al_draw_bitmap(animate(&level->aura.anim),
                       level->aura.hitbox.bounds.center.x + level->aura.pos_graphic.x,
                       level->aura.hitbox.bounds.center.y + level->aura.pos_graphic.y, 0);
        if(level->aura.anim.current_period == level->aura.anim.period - 1)
        {
            level->aura.active = 0;
            level->aura.anim.current_period = 0;
        }
        for(i = 1; i < level->n_characters; i++)
        {
            if(level->characters[i].lives && level->characters[i].inv_timer == 0 && cc_collides(level->aura.hitbox, level->characters[i].hitbox))
            {
                level->characters[i].lives--;
                if(!level->characters[i].lives)
                {
                    if(level->characters[i].type == EnemyOgre)
                        points += 20;
                    else
                        points += 10;
                } else {
                    if(level->characters[i].type == EnemyOgre) {
                        level->characters[i].anims = get_anim(anims, "tiny_zombie");
                        level->characters[i].current = level->characters[i].anims[0];
                        set_character_hitbox(&level->characters[i]);
                    }
                }
                level->characters[i].inv_timer = INV_TIMER;
            }
            if(level->characters[i].inv_timer)
                level->characters[i].inv_timer--;
        }
    }
    if(level->characters[0].atk_timer)
        level->characters[0].atk_timer--;
    for(i = 1; i < level->n_characters; i++)
    {
        if(level->characters[i].inv_timer)
            level->characters[i].inv_timer--;
    }
    return points;
}

void update_ui(LEVEL *level, ALLEGRO_FONT *font, int score)
{
    int i;
    char text[20];
    itoa(score, text, 10);
    printf("\n%s", text);
    for(i = 0; i < 3; i++)
    {
        if(i < level->characters[0].lives)
            al_draw_bitmap(level->heart_full->bitmap, i*16, 0, 0);
        else
            al_draw_bitmap(level->heart_empty->bitmap, i*16, 0, 0);
    }
    al_draw_text(font, al_color_name("yellow"), 960, 0, ALLEGRO_ALIGN_RIGHT, text);
}

int load_mapa(char Nome_Mapa[], LEVEL *level,ANIMATION *anims1,SPRITE *sprite1)
{
    FILE *Arq;

    return Busca(Nome_Mapa, Arq, level, anims1,sprite1);
}

void add_collision(LEVEL *level1, int dx, int dy){
    level1->lines[level1->n_lines].p1.x = level1->envs[(level1->n_envs)].pos_graphic.x + dx;
    level1->lines[level1->n_lines].p1.y = level1->envs[(level1->n_envs)].pos_graphic.y + dy;
    level1->lines[level1->n_lines].p2.x = level1->envs[(level1->n_envs)].pos_graphic.x+16 + dx;
    level1->lines[level1->n_lines].p2.y = level1->envs[(level1->n_envs)].pos_graphic.y + dy;
    set_line_normal(&level1->lines[level1->n_lines], UP);
    set_line_box(&level1->lines[level1->n_lines]);
    level1->n_lines++;

    level1->lines[level1->n_lines].p1.x = level1->envs[(level1->n_envs)].pos_graphic.x + dx;
    level1->lines[level1->n_lines].p1.y = level1->envs[(level1->n_envs)].pos_graphic.y+16 + dy;
    level1->lines[level1->n_lines].p2.x = level1->envs[(level1->n_envs)].pos_graphic.x+16 + dx;
    level1->lines[level1->n_lines].p2.y = level1->envs[(level1->n_envs)].pos_graphic.y+16 + dy;
    set_line_normal(&level1->lines[level1->n_lines], DOWN);
    set_line_box(&level1->lines[level1->n_lines]);
    level1->n_lines++;

    level1->lines[level1->n_lines].p1.x = level1->envs[(level1->n_envs)].pos_graphic.x + dx;
    level1->lines[level1->n_lines].p1.y = level1->envs[(level1->n_envs)].pos_graphic.y + dy;
    level1->lines[level1->n_lines].p2.x = level1->envs[(level1->n_envs)].pos_graphic.x + dx;
    level1->lines[level1->n_lines].p2.y = level1->envs[(level1->n_envs)].pos_graphic.y+16 + dy;
    set_line_normal(&level1->lines[level1->n_lines], LEFT);
    set_line_box(&level1->lines[level1->n_lines]);
    level1->n_lines++;

    level1->lines[level1->n_lines].p1.x = level1->envs[(level1->n_envs)].pos_graphic.x+16 + dx;
    level1->lines[level1->n_lines].p1.y = level1->envs[(level1->n_envs)].pos_graphic.y + dy;
    level1->lines[level1->n_lines].p2.x = level1->envs[(level1->n_envs)].pos_graphic.x+16 + dx;
    level1->lines[level1->n_lines].p2.y = level1->envs[(level1->n_envs)].pos_graphic.y+16 + dy;
    set_line_normal(&level1->lines[level1->n_lines], RIGHT);
    set_line_box(&level1->lines[level1->n_lines]);
    level1->n_lines++;
}

int Busca(char nome_arquivo_inp[MAX_NOME], FILE *arq, LEVEL *level1, ANIMATION *animacao, SPRITE *sprite)
{
    int i=0,j,n_monstro = 0;
    char mapa[LINHA][COLUNA];
    char texto_str[COLUNA];
    char env_name[20];
    int rand_num;

    if(!(arq = fopen((nome_arquivo_inp),"r")))
    {
        return 1;              // Escreve mensagem de erro caso n�o consiga executar a funcao
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
        
        srand(time(NULL));
        for(i=0; i<LINHA; i++)
        {
            level1->characters[(level1->n_characters)].hitbox.bounds.center.x = -1;
            level1->characters[(level1->n_characters)].hitbox.bounds.center.y = -1;
            Coordenada(i,0, &(level1->characters[0].hitbox.bounds.center.x), &(level1->characters[0].hitbox.bounds.center.y), 'J', mapa[i]);
            if(((level1->characters[0].hitbox.bounds.center.x)!= -1)||((level1->characters[0].hitbox.bounds.center.y) != -1))                                  //verifica se a posi��o atual da matriz ainda � zero, caso contrario atualiza o numero de OGROS no mapa
            {
                level1->characters[0].anims = get_anim(animacao, "wizzard_f");
                level1->characters[0].current = level1->characters[0].anims[0];
                level1->characters[0].speed = 3;
                set_character_hitbox(&level1->characters[0]);
                //level1->characters[0].lives = 3;
                level1->characters[0].type = MainCharacter;
                level1->characters[0].inv_timer = 0;
                level1->envs[(level1->n_envs)].pos_graphic = level1->characters[0].hitbox.bounds.center;
                level1->envs[(level1->n_envs)].pos_graphic.x -= 8;
                level1->envs[(level1->n_envs)].pos_graphic.y -= 8;
                level1->envs[(level1->n_envs)].is_anim = 0;
                level1->envs[(level1->n_envs)].sprite = get_sprite(sprite,"floor");
                (level1->n_envs) += 1;
            }

            Coordenada(i,0, &(level1->characters[(level1->n_characters)].hitbox.bounds.center.x), &(level1->characters[(level1->n_characters)].hitbox.bounds.center.y), 'T', mapa[i]);
            if(((level1->characters[(level1->n_characters)].hitbox.bounds.center.x)!= -1)||((level1->characters[(level1->n_characters)].hitbox.bounds.center.y) != -1))                                 //verifica se a posi��o atual da matriz ainda � zero, caso contrario atualiza o numero de OGROS no mapa
            {
                level1->characters[(level1->n_characters)].anims = get_anim(animacao, "big_zombie");
                level1->characters[(level1->n_characters)].current = level1->characters[(level1->n_characters)].anims[0];
                level1->characters[(level1->n_characters)].speed = 0.5;
                set_character_hitbox(&level1->characters[(level1->n_characters)]);
                level1->characters[(level1->n_characters)].lives = 2;
                level1->characters[(level1->n_characters)].type = EnemyOgre;
                level1->envs[(level1->n_envs)].pos_graphic = level1->characters[(level1->n_characters)].hitbox.bounds.center;
                (level1->n_characters) += 1;
                level1->envs[(level1->n_envs)].pos_graphic.x -= 8;
                level1->envs[(level1->n_envs)].pos_graphic.y -= 8;

                level1->envs[(level1->n_envs)].is_anim = 0;
                level1->envs[(level1->n_envs)].sprite = get_sprite(sprite,"floor");
                (level1->n_envs) += 1;
            }

            Coordenada(i,0, &(level1->characters[(level1->n_characters)].hitbox.bounds.center.x), &(level1->characters[(level1->n_characters)].hitbox.bounds.center.y), 'Z', mapa[i]);
            if(((level1->characters[(level1->n_characters)].hitbox.bounds.center.x)!= -1)||((level1->characters[(level1->n_characters)].hitbox.bounds.center.y) != -1))                                  //verifica se a posi��o atual da matriz ainda � zero, caso contrario atualiza o numero de OGROS no mapa
            {
                level1->characters[(level1->n_characters)].anims = get_anim(animacao, "skelet_idle");
                level1->characters[(level1->n_characters)].current = level1->characters[(level1->n_characters)].anims[0];
                level1->characters[(level1->n_characters)].speed = 1;
                set_character_hitbox(&level1->characters[(level1->n_characters)]);
                level1->characters[(level1->n_characters)].lives = 1;
                level1->characters[(level1->n_characters)].type = EnemySkeleton;
                level1->envs[(level1->n_envs)].pos_graphic = level1->characters[(level1->n_characters)].hitbox.bounds.center;

                (level1->n_characters) += 1;
                level1->envs[(level1->n_envs)].pos_graphic.x -= 8;
                level1->envs[(level1->n_envs)].pos_graphic.y -= 8;

                level1->envs[(level1->n_envs)].is_anim = 0;
                level1->envs[(level1->n_envs)].sprite = get_sprite(sprite,"floor");
                (level1->n_envs) += 1;
            }

            for(j=0; j<COLUNA; j++)      // laço para verificar coluna a coluna se ha espinho
            {
                level1->characters[(level1->n_characters)].hitbox.bounds.center.x = -1;
                level1->characters[(level1->n_characters)].hitbox.bounds.center.y = -1;
                Coordenada(i,j, &(level1->characters[(level1->n_characters)].hitbox.bounds.center.x), &(level1->characters[(level1->n_characters)].hitbox.bounds.center.y), 'X', mapa[i]);
                if(((level1->characters[(level1->n_characters)].hitbox.bounds.center.x)!= -1)||((level1->characters[(level1->n_characters)].hitbox.bounds.center.y) != -1))                                 //verifica se a posi��o atual da matriz ainda � zero, caso contrario atualiza o numero de OGROS no mapa
                {
                    level1->characters[(level1->n_characters)].anims = get_anim(animacao, "floor_spikes");
                    level1->characters[(level1->n_characters)].current = level1->characters[(level1->n_characters)].anims[0];
                    level1->characters[(level1->n_characters)].speed = 0;
                    level1->characters[(level1->n_characters)].direction.x = 0;
                    level1->characters[(level1->n_characters)].direction.y = 0;
                    set_character_hitbox(&level1->characters[(level1->n_characters)]);
                    level1->characters[(level1->n_characters)].lives = 1000;
                    level1->characters[(level1->n_characters)].type = EnemySpike;
                    level1->characters[(level1->n_characters)].flags = 0;
                    (level1->n_characters) += 1;
                }
            }

            for(j=0; j<COLUNA; j++)
            {
                level1->envs[(level1->n_envs)].pos_graphic.x = -1;
                level1->envs[(level1->n_envs)].pos_graphic.y = -1;
                Coordenada(i,j, &(level1->envs[(level1->n_envs)].pos_graphic.x), &(level1->envs[(level1->n_envs)].pos_graphic.y), '#', mapa[i]);
                if(((level1->envs[(level1->n_envs)].pos_graphic.x)!= -1)||((level1->envs[(level1->n_envs)].pos_graphic.y) != -1))                                  //verifica se a posi��o atual da matriz ainda � zero, caso contrario atualiza o numero de OGROS no mapa
                {
                    level1->envs[(level1->n_envs)].is_anim = 0;
                    level1->envs[(level1->n_envs)].sprite = get_sprite(sprite,"wall_mid");
                    add_collision(level1, 0, -4);

                    (level1->n_envs) += 1;
                    level1->envs[(level1->n_envs)].pos_graphic = level1->envs[(level1->n_envs) - 1].pos_graphic;
                    level1->envs[(level1->n_envs)].pos_graphic.y -= 16;
                    level1->envs[(level1->n_envs)].is_anim = 0;
                    level1->envs[(level1->n_envs)].sprite = get_sprite(sprite,"wall_top_mid");
                    (level1->n_envs) += 1;
                }
            }

            for(j=0; j<COLUNA; j++)
            {
                level1->envs[(level1->n_envs)].pos_graphic.x = -1;
                level1->envs[(level1->n_envs)].pos_graphic.y = -1;
                Coordenada(i,j, &(level1->envs[(level1->n_envs)].pos_graphic.x), &(level1->envs[(level1->n_envs)].pos_graphic.y), 'w', mapa[i]);
                if(((level1->envs[(level1->n_envs)].pos_graphic.x)!= -1)||((level1->envs[(level1->n_envs)].pos_graphic.y) != -1))                                  //verifica se a posi��o atual da matriz ainda � zero, caso contrario atualiza o numero de OGROS no mapa
                {
                    level1->envs[(level1->n_envs)].is_anim = 0;
                    level1->envs[(level1->n_envs)].sprite = get_sprite(sprite,"wall_mid");
                    add_collision(level1, 0, -4);

                    (level1->n_envs) += 1;
                }
            }

            for(j=0; j<COLUNA; j++)
            {
                level1->envs[(level1->n_envs)].pos_graphic.x = -1;
                level1->envs[(level1->n_envs)].pos_graphic.y = -1;
                Coordenada(i,j, &(level1->envs[(level1->n_envs)].pos_graphic.x), &(level1->envs[(level1->n_envs)].pos_graphic.y), '|', mapa[i]);
                if(((level1->envs[(level1->n_envs)].pos_graphic.x)!= -1)||((level1->envs[(level1->n_envs)].pos_graphic.y) != -1))                                  //verifica se a posi��o atual da matriz ainda � zero, caso contrario atualiza o numero de OGROS no mapa
                {   
                    level1->envs[(level1->n_envs)].is_anim = 0;
                    strcpy(env_name, "floor_");
                    rand_num = rand() % 100;
                    if(rand_num < 93)
                        strcat(env_name, "1");
                    else if(rand_num < 94)
                        strcat(env_name, "2");
                    else if(rand_num < 95)
                        strcat(env_name, "3");
                    else if(rand_num < 96)
                        strcat(env_name, "4");
                    else if(rand_num < 97)
                        strcat(env_name, "5");
                    else if(rand_num < 98)
                        strcat(env_name, "6");
                    else if(rand_num < 99)
                        strcat(env_name, "7");
                    else
                        strcat(env_name, "8");
                    level1->envs[(level1->n_envs)].sprite = get_sprite(sprite, env_name);
                    (level1->n_envs) += 1;

                    level1->envs[(level1->n_envs)].is_anim = 0;
                    level1->envs[(level1->n_envs)].sprite = get_sprite(sprite,"wall_side_mid_right");
                    level1->envs[(level1->n_envs)].pos_graphic = level1->envs[(level1->n_envs - 1)].pos_graphic;
                    add_collision(level1, -12, 0);
                    (level1->n_envs) += 1;

                    
                }
            }

            for(j=0; j<COLUNA; j++)
            {
                level1->envs[(level1->n_envs)].pos_graphic.x = -1;
                level1->envs[(level1->n_envs)].pos_graphic.y = -1;
                Coordenada(i,j, &(level1->envs[(level1->n_envs)].pos_graphic.x), &(level1->envs[(level1->n_envs)].pos_graphic.y), 'l', mapa[i]);
                if(((level1->envs[(level1->n_envs)].pos_graphic.x)!= -1)||((level1->envs[(level1->n_envs)].pos_graphic.y) != -1))                                  //verifica se a posi��o atual da matriz ainda � zero, caso contrario atualiza o numero de OGROS no mapa
                {
                    level1->envs[(level1->n_envs)].is_anim = 0;
                    strcpy(env_name, "floor_");
                    rand_num = rand() % 100;
                    if(rand_num < 93)
                        strcat(env_name, "1");
                    else if(rand_num < 94)
                        strcat(env_name, "2");
                    else if(rand_num < 95)
                        strcat(env_name, "3");
                    else if(rand_num < 96)
                        strcat(env_name, "4");
                    else if(rand_num < 97)
                        strcat(env_name, "5");
                    else if(rand_num < 98)
                        strcat(env_name, "6");
                    else if(rand_num < 99)
                        strcat(env_name, "7");
                    else
                        strcat(env_name, "8");
                    level1->envs[(level1->n_envs)].sprite = get_sprite(sprite, env_name);
                    (level1->n_envs) += 1;
                    level1->envs[(level1->n_envs)].pos_graphic = level1->envs[(level1->n_envs - 1)].pos_graphic;
                    level1->envs[(level1->n_envs)].is_anim = 0;
                    level1->envs[(level1->n_envs)].sprite = get_sprite(sprite,"wall_side_mid_left");
                    add_collision(level1, 12, 0);
                    (level1->n_envs) += 1;
                }
            }

            for(j=0; j<COLUNA; j++)
            {
                level1->envs[(level1->n_envs)].pos_graphic.x = -1;
                level1->envs[(level1->n_envs)].pos_graphic.y = -1;
                Coordenada(i,j, &(level1->envs[(level1->n_envs)].pos_graphic.x), &(level1->envs[(level1->n_envs)].pos_graphic.y), '^', mapa[i]);
                if(((level1->envs[(level1->n_envs)].pos_graphic.x)!= -1)||((level1->envs[(level1->n_envs)].pos_graphic.y) != -1))                                  //verifica se a posi��o atual da matriz ainda � zero, caso contrario atualiza o numero de OGROS no mapa
                {
                    level1->envs[(level1->n_envs)].is_anim = 0;
                    level1->envs[(level1->n_envs)].sprite = get_sprite(sprite,"wall_inner_corner_mid_right");
                    add_collision(level1, 12, 0);
                    add_collision(level1, 0, -4);
                    (level1->n_envs) += 1;
                }
            }

            for(j=0; j<COLUNA; j++)
            {
                level1->envs[(level1->n_envs)].pos_graphic.x = -1;
                level1->envs[(level1->n_envs)].pos_graphic.y = -1;
                Coordenada(i,j, &(level1->envs[(level1->n_envs)].pos_graphic.x), &(level1->envs[(level1->n_envs)].pos_graphic.y), '>', mapa[i]);
                if(((level1->envs[(level1->n_envs)].pos_graphic.x)!= -1)||((level1->envs[(level1->n_envs)].pos_graphic.y) != -1))                                  //verifica se a posi��o atual da matriz ainda � zero, caso contrario atualiza o numero de OGROS no mapa
                {
                    level1->envs[(level1->n_envs)].is_anim = 0;
                    strcpy(env_name, "floor_");
                    rand_num = rand() % 100;
                    if(rand_num < 93)
                        strcat(env_name, "1");
                    else if(rand_num < 94)
                        strcat(env_name, "2");
                    else if(rand_num < 95)
                        strcat(env_name, "3");
                    else if(rand_num < 96)
                        strcat(env_name, "4");
                    else if(rand_num < 97)
                        strcat(env_name, "5");
                    else if(rand_num < 98)
                        strcat(env_name, "6");
                    else if(rand_num < 99)
                        strcat(env_name, "7");
                    else
                        strcat(env_name, "8");
                    level1->envs[(level1->n_envs)].sprite = get_sprite(sprite, env_name);
                    (level1->n_envs) += 1;
                    level1->envs[(level1->n_envs)].pos_graphic = level1->envs[(level1->n_envs - 1)].pos_graphic;
                    level1->envs[(level1->n_envs)].is_anim = 0;
                    level1->envs[(level1->n_envs)].sprite = get_sprite(sprite,"wall_inner_corner_l_top_rigth");
                    add_collision(level1, 12, 0);
                    (level1->n_envs) += 1;
                }
            }

            for(j=0; j<COLUNA; j++)
            {
                level1->envs[(level1->n_envs)].pos_graphic.x = -1;
                level1->envs[(level1->n_envs)].pos_graphic.y = -1;
                Coordenada(i,j, &(level1->envs[(level1->n_envs)].pos_graphic.x), &(level1->envs[(level1->n_envs)].pos_graphic.y), 'v', mapa[i]);
                if(((level1->envs[(level1->n_envs)].pos_graphic.x)!= -1)||((level1->envs[(level1->n_envs)].pos_graphic.y) != -1))                                  //verifica se a posi��o atual da matriz ainda � zero, caso contrario atualiza o numero de OGROS no mapa
                {
                    level1->envs[(level1->n_envs)].is_anim = 0;
                    strcpy(env_name, "floor_");
                    rand_num = rand() % 100;
                    if(rand_num < 93)
                        strcat(env_name, "1");
                    else if(rand_num < 94)
                        strcat(env_name, "2");
                    else if(rand_num < 95)
                        strcat(env_name, "3");
                    else if(rand_num < 96)
                        strcat(env_name, "4");
                    else if(rand_num < 97)
                        strcat(env_name, "5");
                    else if(rand_num < 98)
                        strcat(env_name, "6");
                    else if(rand_num < 99)
                        strcat(env_name, "7");
                    else
                        strcat(env_name, "8");
                    level1->envs[(level1->n_envs)].sprite = get_sprite(sprite, env_name);
                    (level1->n_envs) += 1;
                    level1->envs[(level1->n_envs)].pos_graphic = level1->envs[(level1->n_envs - 1)].pos_graphic;
                    level1->envs[(level1->n_envs)].is_anim = 0;
                    level1->envs[(level1->n_envs)].sprite = get_sprite(sprite,"wall_inner_corner_l_top_left");
                    add_collision(level1, -12, 0);
                    (level1->n_envs) += 1;
                }
            }

            for(j=0; j<COLUNA; j++)
            {
                level1->envs[(level1->n_envs)].pos_graphic.x = -1;
                level1->envs[(level1->n_envs)].pos_graphic.y = -1;
                Coordenada(i,j, &(level1->envs[(level1->n_envs)].pos_graphic.x), &(level1->envs[(level1->n_envs)].pos_graphic.y), '<', mapa[i]);
                if(((level1->envs[(level1->n_envs)].pos_graphic.x)!= -1)||((level1->envs[(level1->n_envs)].pos_graphic.y) != -1))                                  //verifica se a posi��o atual da matriz ainda � zero, caso contrario atualiza o numero de OGROS no mapa
                {
                    level1->envs[(level1->n_envs)].is_anim = 0;
                    level1->envs[(level1->n_envs)].sprite = get_sprite(sprite,"wall_inner_corner_mid_left");
                    add_collision(level1, -12, 0);
                    add_collision(level1, 0, -4);
                    (level1->n_envs) += 1;
                }
            }

            for(j=0; j<COLUNA; j++)
            {
                Coordenada(i,j, &(level1->envs[(level1->n_envs)].pos_graphic.x), &(level1->envs[(level1->n_envs)].pos_graphic.y), ' ', mapa[i]);
                if(((level1->envs[(level1->n_envs)].pos_graphic.x)!= -1)||((level1->envs[(level1->n_envs)].pos_graphic.y) != -1))                                  //verifica se a posi��o atual da matriz ainda � zero, caso contrario atualiza o numero de OGROS no mapa
                {
                    level1->envs[(level1->n_envs)].is_anim = 0;
                    strcpy(env_name, "floor_");
                    rand_num = rand() % 100;
                    if(rand_num < 93)
                        strcat(env_name, "1");
                    else if(rand_num < 94)
                        strcat(env_name, "2");
                    else if(rand_num < 95)
                        strcat(env_name, "3");
                    else if(rand_num < 96)
                        strcat(env_name, "4");
                    else if(rand_num < 97)
                        strcat(env_name, "5");
                    else if(rand_num < 98)
                        strcat(env_name, "6");
                    else if(rand_num < 99)
                        strcat(env_name, "7");
                    else
                        strcat(env_name, "8");
                    level1->envs[(level1->n_envs)].sprite = get_sprite(sprite, env_name);
                    (level1->n_envs) += 1;
                }
            }

            for(j=0; j<COLUNA; j++)
            {
                level1->envs[(level1->n_envs)].pos_graphic.x = -1;
                level1->envs[(level1->n_envs)].pos_graphic.y = -1;
                Coordenada(i,j, &(level1->envs[(level1->n_envs)].pos_graphic.x), &(level1->envs[(level1->n_envs)].pos_graphic.y), 'G', mapa[i]);
                if(((level1->envs[(level1->n_envs)].pos_graphic.x)!= -1)||((level1->envs[(level1->n_envs)].pos_graphic.y) != -1))                                  //verifica se a posi��o atual da matriz ainda � zero, caso contrario atualiza o numero de OGROS no mapa
                {
                    level1->envs[(level1->n_envs)].is_anim = 0;
                    level1->envs[(level1->n_envs)].sprite = get_sprite(sprite,"floor");
                    (level1->n_envs) += 1;
                    level1->envs[(level1->n_envs)].pos_graphic = level1->envs[(level1->n_envs) - 1].pos_graphic;
                    level1->envs[(level1->n_envs)].anim = *get_anim(animacao,"chest_full_open");
                    level1->envs[(level1->n_envs)].anim.period = 1;
                    level1->envs[(level1->n_envs)].is_anim = 1;
                    level1->chest.bounds.center = level1->envs[(level1->n_envs)].pos_graphic;
                    level1->chest.bounds.center.x += 8;
                    level1->chest.bounds.halfwidth = 8;
                    level1->chest.bounds.halfheight = 8;
                    level1->chest.r = 8;
                    (level1->n_envs) += 1;
                }
            }
        }
    }
    return 0;
}

void Coordenada(int linha,int coluna,float *cord_x,float *cord_y, char busca, char Mapa[])
{
    int corre_coluna;
    if((busca != '#') && (busca != ' ') && (busca != 'G') && (busca != 'X') && (busca != '|') && (busca != 'l') && (busca != '^')
         && (busca != '>') && (busca != 'v') && (busca != '<') && (busca != 'w'))
    {
        for(corre_coluna = 0; corre_coluna < COLUNA; corre_coluna ++)
        {
            if(Mapa[corre_coluna] == busca)
            {
                *cord_x = corre_coluna*16+8;
                *cord_y = linha*16+16+8;
            }
        }
    }
    else
    {
        if(busca != 'X')
        {
            if(Mapa[coluna] == busca)
            {
                *cord_x = coluna*16;
                *cord_y = linha*16+16;
            }
        }
        else
        {
            if(Mapa[coluna] == busca)
            {
                *cord_x = coluna*16+8;
                *cord_y = linha*16+16+8;
            }
        }
    }
}

void atualiza_env(LEVEL *level)
{
    for(int i=0; i<(level->n_envs); i++)
    {
        update_env(&level->envs[i]);
    }
}

int got_treasure(LEVEL *level)
{
    return cc_collides(level->characters[0].hitbox, level->chest);
}

int any_monster_alive(LEVEL *level)
{
    int i = 1;
    int alive;
    while((level->characters[i].lives == 0 || level->characters[i].type == EnemySpike) && i < level->n_characters)
        i++;
    if(i == level->n_characters)
        alive = 0;
    else
        alive = 1;
    return alive;
}

int level_over(LEVEL *level)
{
    int status;
    if(!level->characters[0].lives)
        status = GAME_OVER;
    else if(!any_monster_alive(level))
        status = KILLED_MONSTERS;
    else if(got_treasure(level))
        status = GOT_TREASURE;
    else
        status = GO_ON;
    return status;
}

void add_line(LEVEL *level)
{

    LINE wall_south;

    //add retas para colisao
    //muro do sul
    //cordenadas
    wall_south.p1.x = 16;
    wall_south.p1.y = 16+16;
    wall_south.p2.x = 944;
    wall_south.p2.y = 16+16;
    //linha de colisao
    set_line_normal(&wall_south,DOWN);
    //area da linha que diz se tem colisao ou n
    set_line_box(&wall_south);
    level->lines[0] = wall_south;


    wall_south.p1.x = 944;
    wall_south.p1.y = 352+16;
    wall_south.p2.x = 16;
    wall_south.p2.y = 352+16;
    //linha de colisao
    set_line_normal(&wall_south,UP);
    //area da linha que diz se tem colisao ou n
    set_line_box(&wall_south);
    level->lines[1] = wall_south;


    wall_south.p1.x = 16;
    wall_south.p1.y = 16+16;
    wall_south.p2.x = 16;
    wall_south.p2.y = 352+16;
    //linha de colisao
    set_line_normal(&wall_south,DOWN);
    //area da linha que diz se tem colisao ou n
    set_line_box(&wall_south);
    level->lines[2] = wall_south;


    wall_south.p1.x = 944;
    wall_south.p1.y = 352+16;
    wall_south.p2.x = 944;
    wall_south.p2.y = 16+16;
    //linha de colisao
    set_line_normal(&wall_south,LEFT);
    //area da linha que diz se tem colisao ou n
    set_line_box(&wall_south);
    level->lines[3] = wall_south;
    //dizer quantas linhas tem
    level->n_lines = 4;




}
