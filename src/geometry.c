#include <geometry.h>
#include <math.h>


COORD dist_from_to(COORD from, COORD to){
    COORD dist;
    dist.x = to.x - from.x;
    dist.y = to.y - from.y;
    return dist;
}

void add_to_vector(COORD *dst, float x, float y){
    (*dst).x += x;
    (*dst).y += y;
}

float module(COORD v){
    return sqrt(v.x * v.x + v.y * v.y);
}

void normalize(COORD *v){
    float m = module(*v);
    if(m){
        (*v).x /= m;
        (*v).y /= m;
    }
}

COORD input_to_vector(KEYBOARD_STATE input){
    COORD output_vector;
    output_vector.x = 0;
    output_vector.y = 0;
    if(input & UP) add_to_vector(&output_vector, 0, -1);
    if(input & DOWN) add_to_vector(&output_vector, 0, 1);
    if(input & RIGHT) add_to_vector(&output_vector, 1, 0);
    if(input & LEFT) add_to_vector(&output_vector, -1, 0);
    normalize(&output_vector);
    return output_vector;
}

void add_vector(COORD *pos, COORD delta, float speed){
    (*pos).x += delta.x * speed;
    (*pos).y += delta.y * speed;
}


COORD direction_from_to(COORD from, COORD to){
    COORD dist = dist_from_to(from, to);
    normalize(&dist);
    return dist;
}