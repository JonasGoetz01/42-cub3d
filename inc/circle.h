#ifndef CIRCLE_H
# define CIRCLE_H

# include "point.h"

typedef struct  s_circle
{
    t_point *center;
    int     radius;
}               t_circle;

void    circle(t_circle *circle, t_global *global, uint32_t color);

#endif