#ifndef RAY_H
#define RAY_H

#include "point.h"

typedef struct  s_global t_global;

typedef struct  s_ray
{
    double      angle;
    t_point     hit;
}               t_ray;

void    render_rays(t_global *global);
t_ray   *new_ray(double angle);
void    ray_cast(t_global *global, t_ray *ray);

#endif
