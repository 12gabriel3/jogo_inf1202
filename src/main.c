
#include <graphics.h>
#include <input.h>
#include <geometry.h>
#include <character.h>
#include <level.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>


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
    ALLEGRO_BITMAP *fundo = NULL;
    /*                                                     Inicio altera��o sem o gabriel
    -----------------------------------------------------------------------------------------------------------------------------------------------*/
    int i;
    char MAPA[LINHA][COLUNA];

    for(i=0; i>200; i++)
    {
        level.characters[i].hitbox.bounds.center.x = 0;
        level.characters[i].hitbox.bounds.center.y = 0;
    }
    /*                                                     fim altera��o sem o gabriel
    -----------------------------------------------------------------------------------------------------------------------------------------------*/
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
        printf("Nao consegui");
    }
    display = al_create_display(960,368);

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

    //come�ar a contagem de tempo
    al_start_timer(timer);

    //titulo da tela
    al_set_window_title(display, "O mago");

    //pegar sprites
    al_init_image_addon();
    //p representar o muro vermelho
    al_init_primitives_addon();

    //coloca todos os sprites e imagens que est�o na pasta aos vetores
    carrega_sprites(sprites, anims, "../img"); //.. :Diretorio de cima

    //add retas para colisao
    // muro do sul
    wall_south.normal.y = -1;
    wall_south.normal.x = 0;


    level.aura.anim = *get_anim(anims, "aura");
    level.aura.anim.period = 20;
    level.aura.active = 0;

    level.n_characters = 1;
    level.n_envs = 0;
    load_jogo("../Map/FASE 1.txt",MAPA,&level,anims,sprites);

    for(i=0; i<LINHA; i++)
        printf("%s",MAPA[i]);

    Salva_Jogo(MAPA,"../map/Naodeu.txt",&level);

    add_line(&level);
    fundo = al_load_bitmap("../img/Fundo.png");

    level.heart_full = get_sprite(sprites, "ui_heart_full");
    level.heart_empty = get_sprite(sprites, "ui_heart_empty");



    //looping janela allegro p ficar aberta
    do
    {
        //espera ocorrer algo na fila de evento para executar
        al_wait_for_event(queue, &event);//passando o endere�o, vai modificando a cada atualiza��o

        //saber quais teclas est�o sendo apertadas
        set_kb_state(&key_pressed, event);//&key referencia de quais teclas est�o sendo usadas

        //diz para o nivel qual o imput
        level.input = key_pressed; //agora o nivel vai saber qual tecla esta sendo apertada

        //analisa o evento timer p cada frame
        if(event.type == ALLEGRO_EVENT_TIMER)
        {
            //limpa a tela p preto (tabela RGB 000) p cada atualiza��o
            al_clear_to_color(al_map_rgb_f(0, 0, 0));
            al_draw_bitmap_region(fundo,0,0,960,368,0,0,0);
            atualiza_env(&level);

            //atualiza as caracteristicas de cada char
            update_characters(&level);
            get_main_collision(&level);
            update_ui(&level);
            atk(&level);
            //desenha a linha vermelha p allegro
           // al_draw_line(0, 400, 600, 500, al_color_name("red"), 1);



            //atualiza tela (?) veio de um exemplo
            al_flip_display();
        }
    }
    //A allegro roda ate tu apertar 'X' p encerrar o prog
    while(event.type != ALLEGRO_EVENT_DISPLAY_CLOSE);

    return 0;
}
