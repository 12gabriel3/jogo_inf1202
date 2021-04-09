#include <graphics.h>
#include <input.h>
#include <geometry.h>
#include <character.h>

#define SPEED 3
#define ANIM_FRAMES 10
#define SPEED_DIAGONAL (SPEED*0.70710678118)

int main(){
    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_EVENT event;
    ALLEGRO_KEYBOARD_STATE *ret_state;
    SPRITE sprites[SPRITES_MAX];
    ANIMATION anims[ANIMS_MAX];
    CHARACTER mage, demon, zombie;
    COORD pos_mage = {320, 240};
    COORD pos_demon = {0, 0};
    COORD pos_zombie = {600, 450};
    KEYBOARD_STATE key_pressed;
    ALLEGRO_MONITOR_INFO monitor;

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
    carrega_sprites(sprites, anims, "../img");

    // Personagem principal
    mage.anims = get_anim(anims, "wizzard_m");
    mage.current = mage.anims[0];
    mage.speed = 3;
    mage.pos_graphic = pos_mage;
    mage.alive = 1;
    mage.type = MainCharacter;

    demon.anims = get_anim(anims, "big_demon");
    demon.current = demon.anims[0];
    demon.speed = 1;
    demon.pos_graphic = pos_demon;
    demon.alive = 1;
    demon.type = EnemyOgre;

    zombie.anims = get_anim(anims, "big_zombie");
    zombie.current = zombie.anims[0];
    zombie.speed = 2;
    zombie.pos_graphic= pos_demon;
    zombie.alive = 1;
    zombie.type = EnemyOgre;

    do{
        al_wait_for_event(queue, &event);
        if(event.type == ALLEGRO_EVENT_TIMER){
            al_clear_to_color(al_map_rgb_f(0, 0, 0));

            update_character(&mage, key_pressed, mage.hitbox.center);
            update_character(&demon, key_pressed, mage.hitbox.center);
            update_character(&zombie, key_pressed, mage.hitbox.center);
            al_flip_display();
        }
        set_kb_state(&key_pressed, event);
    }while(event.type != ALLEGRO_EVENT_DISPLAY_CLOSE);

    return 0;
}