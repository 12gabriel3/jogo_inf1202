
#include <graphics.h>
#include <input.h>
#include <geometry.h>
#include <character.h>
#include <level.h>
#include <game.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


int main()
{
    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    //fila de eventos p serem processados, organiza tudo
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_EVENT event;
    GAME game;

    //intalar as coisas do allegro
    if(!al_init())
    {
        return -1;
    }

    al_install_keyboard();
    al_install_mouse();

    display = al_create_display(960,384);

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
    al_init_font_addon();
    al_init_ttf_addon();
    //coloca todos os sprites e imagens que est�o na pasta aos vetores
    carrega_sprites(game.sprites, game.anims, "../img"); //.. :Diretorio de cima
    
    game.state = PAUSE;
    game.font = al_load_font("../fonts/PressStart2P-Regular.ttf", -20, 0);
    //looping janela allegro p ficar aberta
    do
    {
        //espera ocorrer algo na fila de evento para executar
        al_wait_for_event(queue, &event);//passando o endere�o, vai modificando a cada atualiza��o

    }
    //A allegro roda ate tu apertar 'X' p encerrar o prog
    while(event.type != ALLEGRO_EVENT_DISPLAY_CLOSE && run_game(&game, event));

    return 0;
}
