#include "../../inc/cub3d.h"

void    make_background_transparent(t_global *global)
{
    uint32_t x;
    uint32_t y;

    y = 0;
    while (y < global->minimap->height)
    {
        x = 0;
        while (x < global->minimap->width)
        {
            mlx_put_pixel(global->minimap, x, y, get_rgba(0, 0, 0, 100));
            x++;
        }
        y++;
    }
}

void    show_sky_and_floor(t_global *global)
{
    int x;
    int y;

    y = 0;
    while (y < global->window_height / 2)
    {
        x = 0;
        while (x < global->window_width)
        {
            mlx_put_pixel(global->img, x, y, get_rgba(0, 255, 255, 255));
            x++;
        }
        y++;
    }
    while (y < global->window_height)
    {
        x = 0;
        while (x < global->window_width)
        {
            mlx_put_pixel(global->img, x, y, get_rgba(255, 255, 0, 255));
            x++;
        }
        y++;
    }
}

float map_distance_to_height(float distance, t_global *global)
{
    float height;
    int window_height;

    window_height = global->window_height;

    if (distance == 0)
        height = window_height;
    else
        height = (global->scale_factor / distance) * window_height;

    if (height > window_height)
        height = window_height;

    return height;
}


float   get_distance(t_vec2d a, t_vec2d b)
{
    return (sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2)));
}

int get_wall_color(t_face face)
{
    if (face == NORTH)
        return NORTH_COLOR;
    if (face == SOUTH)
        return SOUTH_COLOR;
    if (face == EAST)
        return EAST_COLOR;
    if (face == WEST)
        return WEST_COLOR;
    return get_rgba(255, 255, 255, 255); // Default color if something goes wrong
}

void render_3d(t_global *global)
{
    int bar_width = global->img->width / global->img->width;

    for (int i = 0; i < (int)global->img->width; i++)
    {
        t_ray *ray = &global->player->rays[i];
        t_collision *closest_collision = ray->closest_collision;

        if (closest_collision)
        {
            float distance = get_distance(global->player->pos, closest_collision->point);
            int bar_height = map_distance_to_height(distance, global);
            int center_y = global->img->height / 2;
            int top_y = center_y - (bar_height / 2);
            int x = i * bar_width;

            int color = get_wall_color(closest_collision->face);
            draw_bar(global, x, top_y, bar_width, bar_height, color);
        }
    }
}

