#include <geometry.h>
#include <math.h>

COORD dist_from_to(COORD from, COORD to)
{
    COORD dist;
    dist.x = to.x - from.x;
    dist.y = to.y - from.y;
    return dist;
}

void add_to_vector(COORD *dst, float x, float y)
{
    dst->x += x;
    dst->y += y;
}

float module(COORD v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

void normalize(COORD *v)
{
    float m = module(*v);
    if(m)
    {
        v->x /= m;
        v->y /= m;
    }
}

COORD input_to_vector(KEYBOARD_STATE input)
{
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

void add_vector(COORD *pos, COORD delta, float speed)
{
    if(module(delta) > 0.5)
    {
        pos->x += delta.x * speed;
        pos->y += delta.y * speed;
    }
}


COORD direction_from_to(COORD from, COORD to)
{
    COORD dist = dist_from_to(from, to);
    if(module(dist) > 1)
    {
        normalize(&dist);
    }
    else
    {
        dist.x = 0;
        dist.y = 0;
    }
    return dist;
}

//linha definida por 2 pontos que gera
float shortest_to_line(LINE l, COORD p)
{
    // https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
    float n1 = (l.p2.x - l.p1.x) * (l.p1.y - p.y);
    float n2 = (l.p1.x - p.x) * (l.p2.y - l.p1.y);
    float d1 = pow(l.p2.x - l.p1.x, 2);
    float d2 = pow(l.p2.y - l.p1.y, 2);
    return fabs(n1 - n2) / pow(d1 + d2, 0.5);
}

COORD lc_collision_normal(HITBOX c, LINE l)
{
    COORD normal = {0, 0};
    HITBOX corner;
    corner.r = 0;
    // reta horizontal
    if(l.p1.y == l.p2.y)
    {
        // Se o centro do circulo estiver no meio da reta
        if(c.center.x > l.p1.x && c.center.x < l.p2.x)
        {
            if(shortest_to_line(l, c.center) < c.r) normal = l.normal;
        }
        else if(c.center.x <= l.p1.x)
        {
            corner.center = l.p1;
            normal = cc_collision_normal(corner, c);
        }
        else
        {
            corner.center = l.p2;
            normal = cc_collision_normal(corner, c);
        }
    }
    // reta vertical
    else
    {
        // Se o centro do circulo estiver no meio da reta
        if(c.center.y > l.p1.y && c.center.y < l.p2.y)
        {
            if(shortest_to_line(l, c.center) < c.r) normal = l.normal;
        }
        else if(c.center.y <= l.p1.y)
        {
            corner.center = l.p1;
            normal = cc_collision_normal(corner, c);
        }
        else
        {
            corner.center = l.p2;
            normal = cc_collision_normal(corner, c);
        }
    }
    return normal;
}

COORD cc_collision_normal(HITBOX c1, HITBOX c2)
{
    COORD normal;
    normal = dist_from_to(c2.center, c1.center);
    if(module(normal) > c1.r + c2.r)
    {
        normal.x = 0;
        normal.y = 0;
    }
    else
    {
        normalize(&normal);
    }
    return normal;
}

float dot_prod(COORD v1, COORD v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

COORD multiply(float scalar, COORD v)
{
    COORD mult;
    mult.x = v.x * scalar;
    mult.y = v.y * scalar;
    return mult;
}

void subtract(COORD *v1, COORD v2)
{
    v1->x -= v2.x;
    v1->y -= v2.y;
}

void vec_sum(COORD *v1, COORD v2)
{
    v1->x += v2.x;
    v1->y += v2.y;
}

COORD project_a_on_b(COORD a, COORD b)
{
    float dot = dot_prod(a, b) / dot_prod(b, b);
    if(dot < 0) dot = 0;
    // Projeção = (a . b) / (b . b) * b
    return multiply(dot, b);
}

void rm_direction(COORD direction, COORD *vector)
{
    subtract(vector, project_a_on_b(*vector, direction));
}
