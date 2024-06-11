#ifndef COLLISION_H
#define COLLISION_H

#include "point.h"

typedef struct  s_global t_global;

typedef struct  s_collision
{
    t_point     *pos;
    double      distance;
    t_collision *next;
}               t_collision;

t_collision *new_collision(t_point *pos, double distance);
void    free_collision(t_collision *collision);



#endif
