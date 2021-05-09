#include <env.h>


void update_env(ENV *col)
{
    if(col->anim)
        al_draw_bitmap(animate(col->anim),
                       col->pos_graphic.x,
                       col->pos_graphic.y,
                       0);
    else
        al_draw_bitmap(col->sprite->bitmap,
                       col->pos_graphic.x,
                       col->pos_graphic.y,
                       0);
}


