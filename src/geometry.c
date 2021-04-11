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

/**
 * @brief Diz se o trianglulo é obtuso e indica qual o ponto
 * 
 * @param p0 as coordenadas do personagem
 * @param p1 as coordenadas de um dos vertices
 * @param p2 as coordenadas de um dos vertices
 * @param point as coordenadas do ponto obtuso
 * @return indica se é ou não obtuso
 */
int obtuse_angle(COORD p0, COORD p1, COORD p2, COORD *point){
    int is_obtuse = 0;
    float p0p1 = module(dist_from_to(p0, p1));
    float p0p2 = module(dist_from_to(p0, p2));
    float p1p2 = module(dist_from_to(p1, p2));
    float p0p1sq = p0p1 * p0p1;
    float p0p2sq = p0p2 * p0p2;
    float p1p2sq = p1p2 * p1p2;
    if(p0p1sq > p1p2sq + p0p2sq){
        is_obtuse = 1;
        *point = p2;
    }
    else if(p0p2sq > p1p2sq + p0p1sq){
        is_obtuse = 1;
        *point = p1;
    }
    return is_obtuse;
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

float shortest_to_line(LINE l, COORD p){
    // https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
    float n1 = (l.p2.x - l.p1.x) * (l.p1.y - p.y);
    float n2 = (l.p1.x - p.x) * (l.p2.y - l.p1.y);
    float d1 = pow(l.p2.x - l.p1.x, 2);
    float d2 = pow(l.p2.y - l.p1.y, 2);
    return fabs(n1 - n2) / pow(d1 + d2, 0.5);
}

COORD invert(COORD c){
    COORD inv;
    inv.x = -c.x;
    inv.y = -c.y;
    return inv;
}

COORD lc_collision_normal(CIRCLE c, LINE l){
    COORD normal = {0, 0};
    CIRCLE corner;
    if(sweep_and_prune(c.bounds, l.bounds)){
        corner.r = 0;
        // Se for obtuso
        if(obtuse_angle(c.bounds.center, l.p1, l.p2, &corner.bounds.center)){
            normal = cc_collision_normal(corner, c);
        }
        else if(shortest_to_line(l, c.bounds.center) < c.r){
            normal = invert(l.normal);
        }
    }
    return normal;
}

COORD change_direction(COORD v, char direction){
    if(direction & UP) v.y = -fmod(v.y, 1.0);
    else v.y = fmod(v.y, 1.0);
    if(direction & LEFT) v.x = -fmod(v.x, 1.0);
    else v.x = fmod(v.x, 1.0);
    return v;
}

void set_line_normal(LINE *l, char direction){
    float aux;
    l->normal = direction_from_to(l->p1, l->p2);
    aux = l->normal.x;
    l->normal.x = l->normal.y;
    l->normal.y = aux;
    l->normal = change_direction(l->normal, direction);
}

void set_line_box(LINE *l){
    l->bounds.halfwidth = fmod(l->p1.x - l->p2.x, 1.0)/2;
    l->bounds.halfheight = fmod(l->p1.x - l->p2.x, 1.0)/2;
    l->bounds.center = multiply(0.5, dist_from_to(l->p1, l->p2));
    vec_sum(&l->bounds.center, l->p1);
}

void set_circle_box(CIRCLE *c){
    c->bounds.halfheight = c->r;
    c->bounds.halfwidth = c->r;
}

int sweep_and_prune(BOX b1, BOX b2){
    int possible_collision = 0;
    if(fmod(b1.center.x - b2.center.x, 1.0) <= b1.halfwidth + b2.halfwidth &&
       fmod(b1.center.y - b2.center.y, 1.0) <= b1.halfheight + b2.halfheight)
       possible_collision = 1;
    return possible_collision;
}

COORD cc_collision_normal(CIRCLE c1, CIRCLE c2){
    COORD normal= {0, 0};
    COORD distance;
    float moddistance;
    BOX b1, b2;
    b1.center = c1.bounds.center;
    b1.halfheight = c1.r;
    b1.halfwidth = c1.r;
    b2.center = c2.bounds.center;
    b2.halfheight = c2.r;
    b2.halfwidth = c2.r;
    if(sweep_and_prune(b1, b2)){
        distance = dist_from_to(c2.bounds.center, c1.bounds.center);
        moddistance = module(distance);
        if(moddistance > c1.r + c2.r) {
            normal.x = 0;
            normal.y = 0;
        } else {
            normal = distance;
            normalize(&normal);
            normal = multiply( c1.r + c2.r - moddistance, normal);
        }
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