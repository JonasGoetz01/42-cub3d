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
            float hit_percentage;
            // based on the closest_collision->line and closest_collision->point calculate the percentage on the hitpoint on the line
            // use this percentage to calculate the alpha value of the color, 0% in the wall should be 0 alpha, 100% in the wall should be 255 alpha
            if (closest_collision->line->alignment == VERTICAL)
            {
                hit_percentage = (closest_collision->point.y - closest_collision->line->a.y) /
                                 (closest_collision->line->b.y - closest_collision->line->a.y);
            }
            else
            {
                hit_percentage = (closest_collision->point.x - closest_collision->line->a.x) /
                                 (closest_collision->line->b.x - closest_collision->line->a.x);
            }
            int alpha = (int)(255 * hit_percentage);
            int color = get_wall_color(closest_collision->face);
            color = get_rgba((color >> 24) & 0xFF, (color >> 16) & 0xFF, (color >> 8) & 0xFF, alpha);
            // mlx_texture_t *texture = mlx_load_png("textures/cobblestone.png");
            
            for (int i = 0; i < bar_width; i++)
            {
                if (x + i < 0 || (uint32_t)x + (uint32_t)i >= global->img->width)
                    continue;
                for (int j = 0; j < bar_height; j++)
                {
                    int draw_y = top_y + j;
                    if (draw_y >= 0 && (uint32_t)draw_y < global->img->height)
                    {
                        mlx_put_pixel(global->img, x + i, draw_y, color);
                    }
                }
            }
        }
    }
}

