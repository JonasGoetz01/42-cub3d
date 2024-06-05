#ifndef CIRCLE_H
# define CIRCLE_H

# include "point.h"

typedef struct  s_circle
{
    t_point *center;
    int     radius;
}               t_circle;

void    circle(t_circle *circle, t_global *global, uint32_t color);
void    circle_point(int x, int y, int radius, t_global *global, uint32_t color);

#endif