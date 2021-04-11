#include <graphics.h>
#include <input.h>
#include <geometry.h>
#include <character.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>

#define N_ENV 10

int main(){
    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_EVENT event;
    ALLEGRO_KEYBOARD_STATE *ret_state;
    SPRITE sprites[SPRITES_MAX];
    ANIMATION anims[ANIMS_MAX];
    CHARACTER characters[3];
    COORD pos_mage = {320, 240};
    COORD pos_demon = {0, 0};
    COORD pos_zombie = {600, 450};
    KEYBOARD_STATE key_pressed;
    ALLEGRO_MONITOR_INFO monitor;
    LINE wall_south;
    
    if(!al_init()){
        return -1;
    }
    al_install_keyboard();
    al_install_mouse();
    if(al_get_monitor_info(0, &monitor)){
        printf("%d, %d", monitor.x2, monitor.y2);
    } else {
        printf("Não consegui");
    }
    display = al_create_display((int) monitor.x2*0.6, (int) monitor.y2 * 0.6);
    
    if (!display) {
       return -1;
    }
    timer = al_create_timer(1.0 / FPS);
    queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
    al_set_window_title(display, "O mago");

    al_init_image_addon();
    al_init_primitives_addon();

    carrega_sprites(sprites, anims, "../img");

    // Personagem principal
    characters[0].anims = get_anim(anims, "wizzard_m");
    characters[0].current = characters[0].anims[0];
    characters[0].speed = 3;
    set_character_hitbox(&characters[0]);
    characters[0].alive = 1;
    characters[0].type = MainCharacter;

    characters[1].anims = get_anim(anims, "big_demon");
    characters[1].current = characters[1].anims[0];
    characters[1].speed = 1;
    set_character_hitbox(&characters[1]);
    characters[1].alive = 1;
    characters[1].type = EnemySkeleton;
    characters[1].hitbox.center.x = 600;
    characters[1].hitbox.center.y = 400;

    characters[2].anims = get_anim(anims, "big_zombie");
    characters[2].current = characters[2].anims[0];
    characters[2].speed = 2;
    set_character_hitbox(&characters[2]);
    characters[2].alive = 1;
    characters[2].type = EnemyOgre;
    characters[2].hitbox.center.x = 600;
    characters[2].hitbox.center.y = 0;

    // muro do sul
    wall_south.normal.y = -1;
    wall_south.normal.x = 0;
    wall_south.p1.x = 0;
    wall_south.p2.x = 600;
    wall_south.p1.y = 400;
    wall_south.p2.y = 400;

    do{
        al_wait_for_event(queue, &event);
        if(event.type == ALLEGRO_EVENT_TIMER){
            al_clear_to_color(al_map_rgb_f(0, 0, 0));
            update_character(&characters[0], key_pressed, characters, wall_south);
            update_character(&characters[1], key_pressed, characters, wall_south);
            update_character(&characters[2], key_pressed, characters, wall_south);
            al_draw_line(0, 400, 600, 400, al_color_name("red"), 1);
            al_flip_display();
        }
        set_kb_state(&key_pressed, event);
    }while(event.type != ALLEGRO_EVENT_DISPLAY_CLOSE);

    return 0;
}