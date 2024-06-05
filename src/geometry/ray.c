#include "../../inc/cub3d.h"

t_ray   *new_ray(t_point origin, double angle)
{
    t_ray *ray = malloc(sizeof(t_ray));

    ray->origin = origin;
    ray->angle = angle;
    ray->hit = (t_point){0, 0};
    return (ray);
}

void    free_ray(t_ray *ray)
{
    free(ray);
}

// void    ray_cast(t_global *global, t_ray *ray)
// {

// }

// draw a line from the origin to the border of the image
void    render_rays(t_global *global)
{
    for (int i = 0; i < NUM_RAYS; i++)
    {
        t_ray *ray = global->player->rays[i];
        line_angle((t_point){ray->origin.x * global->xfactor, ray->origin.y * global->yfactor}, ray->angle, get_rgba(0, 255, 0, 255), global);
    }
}

