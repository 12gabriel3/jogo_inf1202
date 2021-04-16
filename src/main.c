#include <graphics.h>
#include <input.h>
#include <geometry.h>
#include <character.h>
#include <level.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>

//al_ :Funcoes prontas do Allegro

int main()
{
    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    //fila de eventos p serem processados, organiza tudo
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_EVENT event;
    SPRITE sprites[SPRITES_MAX];
    ANIMATION anims[ANIMS_MAX];
    LEVEL level;
    KEYBOARD_STATE key_pressed;
    ALLEGRO_MONITOR_INFO monitor;
    LINE wall_south;
    int i;
    //intalar as coisas do allegro
    if(!al_init())
    {
        return -1;
    }

    al_install_keyboard();
    al_install_mouse();
    if(al_get_monitor_info(0, &monitor))
    {
        printf("%d, %d", monitor.x2, monitor.y2);
    }
    else
    {
        printf("Não consegui");
    }
    display = al_create_display((int) monitor.x2*0.6, (int) monitor.y2 * 0.6);

    if (!display)
    {
        return -1;
    }


    timer = al_create_timer(1.0 / FPS); //quanto tempo a tela sera atualizada
    //inicia os eventos
    queue = al_create_event_queue();

    //local de onde vai pegar os eventos
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    //começar a contagem de tempo
    al_start_timer(timer);

    //titulo da tela
    al_set_window_title(display, "O mago");

    //pegar sprites
    al_init_image_addon();
    //p representar o muro vermelho
    al_init_primitives_addon();

    //coloca todos os sprites e imagens que estão na pasta aos vetores
    carrega_sprites(sprites, anims, "../img"); //.. :Diretorio de cima

    //add retas para colisao
    // muro do sul
    wall_south.normal.y = -1;
    wall_south.normal.x = 0;

    //Personagem principal
    level.characters[0].anims = get_anim(anims, "wizzard_m");
    //current é animaçao atual do main
    level.characters[0].current = level.characters[0].anims[0];
    //velocidade dele
    level.characters[0].speed = 3;
    //para detectar colisoes do main
    set_character_hitbox(&level.characters[0]);
    //dectar a vida do main
    level.characters[0].alive = 1;
     //padronizar o comportamento de um ps
    level.characters[0].type = MainCharacter;

    for(i = 1; i < 10; i++)
    {
        level.characters[i].anims = get_anim(anims, "big_demon");
        level.characters[i].current = level.characters[i].anims[0];
        level.characters[i].speed = 1;
        set_character_hitbox(&level.characters[i]);
        level.characters[i].alive = 1;
        level.characters[i].type = EnemySkeleton;
        //determinam a posicao do ps, seu centro x y
        level.characters[i].hitbox.bounds.center.x = 600;
        level.characters[i].hitbox.bounds.center.y = i * 50 - 100;
    }

    //add os chars no nivel
    for(i = 10; i < 20; i++)
    {
        level.characters[i].anims = get_anim(anims, "big_zombie");
        level.characters[i].current = level.characters[i].anims[0];
        level.characters[i].speed = 2;
        set_character_hitbox(&level.characters[i]);
        level.characters[i].alive = 1;
        level.characters[i].type = EnemyOgre;
        level.characters[i].hitbox.bounds.center.x = 300;
        level.characters[i].hitbox.bounds.center.y = i * 50 - 900;
    }
    level.n_characters = 20;

    //add retas para colisao
    //muro do sul
    //cordenadas
    wall_south.p1.x = 0;
    wall_south.p1.y = 400;
    wall_south.p2.x = 600;
    wall_south.p2.y = 500;
    //linha de colisao
    set_line_normal(&wall_south, UP | RIGHT);
    //area da linha que diz se tem colisao ou n
    set_line_box(&wall_south);
    level.lines[0] = wall_south;
    //dizer quantas linhas tem
    level.n_lines = 1;

    //looping janela allegro p ficar aberta
    do
    {
        //espera ocorrer algo na fila de evento para executar
        al_wait_for_event(queue, &event);//passando o endereço, vai modificando a cada atualização

        //saber quais teclas estão sendo apertadas
        set_kb_state(&key_pressed, event);//&key referencia de quais teclas estão sendo usadas

        //diz para o nivel qual o imput
        level.input = key_pressed; //agora o nivel vai saber qual tecla esta sendo apertada

        //analisa o evento timer p cada frame
        if(event.type == ALLEGRO_EVENT_TIMER)
        {
            //limpa a tela p preto (tabela RGB 000) p cada atualização
            al_clear_to_color(al_map_rgb_f(0, 0, 0));

            //atualiza tela (?)
            al_flip_display();
        
            //direcao que os char querem ir
            set_characters_intention(&level);
            //analisar e dizer que nao pode ir naquela direcao quando houver colisao
            remove_collision(&level);
            //atualiza as caracteristicas de cada char
            update_characters(&level);
            //desenha a linha vermelha p allegro
            al_draw_line(0, 400, 600, 500, al_color_name("red"), 1);
            //atualiza tela (?) veio de um exemplo
            al_flip_display();
        }
    }
    //A allegro roda ate tu apertar 'X' p encerrar o prog
    while(event.type != ALLEGRO_EVENT_DISPLAY_CLOSE);

    return 0;
}
