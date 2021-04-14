#include <level.h>

void set_characters_intention(LEVEL *level){
    int i;
    for(i = 0; i < level->n_characters; i++){
        switch(level->characters[i].type){
            case MainCharacter:
                level->characters[i].direction = input_to_vector(level->input);
                break;
            case EnemySkeleton:
                level->characters[i].direction = direction_from_to(level->characters[i].hitbox.bounds.center, level->characters[0].hitbox.bounds.center);
                if(module(dist_from_to(level->characters[i].hitbox.bounds.center, level->characters[0].hitbox.bounds.center)) < 200) level->characters[i].speed = 2.7;
                else level->characters[i].speed = 1;
                break;
            case EnemyOgre:
                level->characters[i].direction = direction_from_to(level->characters[i].hitbox.bounds.center, level->characters[0].hitbox.bounds.center);
                break;
            case EnemySpike:
                break;
        }
    }
    
}

int remove_collision(LEVEL *level){
    COORD normal;
    CIRCLE next_pos_i, next_pos_j;
    int i, j, collision;
    collision = 0;
    for(i = 0; i < level->n_characters; i++){
        for(j = i; j < level->n_characters; j++){
            if(&level->characters[j] != &level->characters[i]){
                next_pos_i = level->characters[i].hitbox;
                vec_sum(&next_pos_i.bounds.center, multiply(level->characters[i].speed, level->characters[i].direction));
                next_pos_j = level->characters[j].hitbox;
                vec_sum(&next_pos_j.bounds.center, multiply(level->characters[j].speed, level->characters[j].direction));
                normal = cc_collision_normal(next_pos_j, next_pos_i);
                if(normal.x || normal.y){
                    normal = multiply(-1.0/2, normal);
                    vec_sum(&level->characters[i].direction, normal);
                    collision = 1;
                }
                    
            }
        }
        
        for(j = 0; j < level->n_lines; j++){
            next_pos_i = level->characters[i].hitbox;
            vec_sum(&next_pos_i.bounds.center, multiply(level->characters[i].speed, level->characters[i].direction));
            normal = lc_collision_normal(next_pos_i, level->lines[j]);
            if(normal.x || normal.y){
                vec_sum(&level->characters[i].direction, multiply(-0.5, normal));
                collision = 1;
            }
                
        }
    }
    return collision;
    
}

void update_characters(LEVEL *level){
    int i;
    for(i = 0; i < level->n_characters; i++){
        move_character(&level->characters[i]);
        al_draw_bitmap(animate(&level->characters[i].current), 
                       level->characters[i].pos_graphic.x + level->characters[i].hitbox.bounds.center.x, 
                       level->characters[i].pos_graphic.y + level->characters[i].hitbox.bounds.center.y, 
                       level->characters[i].flags);
    }
}

