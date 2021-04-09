#include <env.h>


void update_collidable(COLLIDABLE *col){
    if(col->anim)
        al_draw_bitmap(animate(col->anim), 
                                col->pos_graphic.x + col->hitbox.center.x, 
                                col->pos_graphic.y + col->hitbox.center.y, 
                                0);
    else al_draw_bitmap(col->sprite->bitmap, 
                                col->pos_graphic.x + col->hitbox.center.x, 
                                col->pos_graphic.y + col->hitbox.center.y, 
                                0);
}