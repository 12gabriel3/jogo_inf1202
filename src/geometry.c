#include <geometry.h>
#include <math.h>


COORD dist_from_to(COORD from, COORD to){
    COORD dist;
    dist.x = to.x - from.x;
    dist.y = to.y - from.y;
    return dist;
}

void add_to_vector(COORD *dst, float x, float y){
    dst->x += x;
    dst->y += y;
}

float module(COORD v){
    return sqrt(v.x * v.x + v.y * v.y);
}

void normalize(COORD *v){
    float m = module(*v);
    if(m){
        v->x /= m;
        v->y /= m;
    }
}

int detect_collision(HITBOX h1, HITBOX h2){
    
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
    if(module(delta) > 0.5){
        pos->x += delta.x * speed;
        pos->y += delta.y * speed;
    }
}


COORD direction_from_to(COORD from, COORD to){
    COORD dist = dist_from_to(from, to);
    if(module(dist) > 1){
        normalize(&dist);
    } else {
        dist.x = 0;
        dist.y = 0;
    }
    return dist;
}

int cr_collides(HITBOX c, HITBOX r){
    int collides = 0;
    if(
        // Se colide na coordenada y
        abs(c.center.y - r.center.y) < (c.radius + r.halfheight) &&
        // e na coordenada x
        abs(c.center.x - r.center.x) < (c.radius + r.halfwidth)
    ) collides = 1;
    return collides;
}

int cc_collides(HITBOX c1, HITBOX c2){
    int collides = 0;
    // Se a distancia for menor que a soma dos modulos, esta colidindo
    if(module(dist_from_to(c1.center, c2.center)) < (c1.radius + c2.radius)) collides = 1;
    return collides;
}

COORD cc_colision_normal(HITBOX c1, HITBOX c2){
    COORD normal;
    if(cc_collides(c1, c2)){
        normal = dist_from_to(c2.center, c1.center);
        normalize(&normal);
    }
    return normal;
}

float dot_prod(COORD v1, COORD v2){
    return v1.x * v2.x + v1.y * v2.y;
}

COORD multiply(float scalar, COORD v){
    COORD mult;
    mult.x = v.x * scalar;
    mult.y = v.y * scalar;
    return mult;
}

void subtract(COORD *v1, COORD v2){
    v1->x -= v2.x;
    v1->y -= v2.y;
}

void vec_sum(COORD *v1, COORD v2){
    v1->x += v2.x;
    v1->y += v2.y;
}

COORD project_a_on_b(COORD a, COORD b){
    float dot = dot_prod(a, b) / dot_prod(b, b);
    if(dot < 0) dot = 0;
    // Projeção = (a . b) / (b . b) * b
    return multiply(dot, b);
}

void rm_direction(COORD direction, COORD *vector){
    subtract(vector, project_a_on_b(*vector, direction));
}