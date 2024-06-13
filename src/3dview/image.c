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
    int bar_width;
    int i;
    t_ray *ray;
    t_collision *closest_collision;
    float distance;
    int bar_height;
    int center_y;
    int top_y;
    int x;
    float hit_percentage;
    int color;
    int texture_x, texture_y;
    mlx_texture_t *texture;

    bar_width = global->img->width / global->img->width;
    mlx_texture_t *texture_north = mlx_load_png("textures/cobblestone.png");
    mlx_texture_t *texture_south = mlx_load_png("textures/dirt.png");
    mlx_texture_t *texture_east = mlx_load_png("textures/polished_granite.png");
    mlx_texture_t *texture_west = mlx_load_png("textures/piston_bottom.png");

    i = 0;
    while (i < (int)global->img->width)
    {
        ray = &global->player->rays[i];
        closest_collision = ray->closest_collision;

        if (closest_collision)
        {
            distance = get_distance(global->player->pos, closest_collision->point);
            bar_height = map_distance_to_height(distance, global);
            center_y = global->img->height / 2;
            top_y = center_y - (bar_height / 2);
            x = i * bar_width;
            switch (closest_collision->face)
            {
                case NORTH:
                    texture = texture_north;
                    break;
                case SOUTH:
                    texture = texture_south;
                    break;
                case EAST:
                    texture = texture_east;
                    break;
                case WEST:
                    texture = texture_west;
                    break;
                default:
                    texture = texture_north;
                    break;
            }
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
            texture_x = (int)(hit_percentage * (texture->width - 1));
            for (int j = 0; j < bar_height; j++)
            {
                texture_y = (int)(((float)j / bar_height) * (texture->height - 1));
                uint8_t *pixel = &texture->pixels[(texture_y * texture->width + texture_x) * texture->bytes_per_pixel];
                color = get_rgba(pixel[0], pixel[1], pixel[2], 255);
                for (int k = 0; k < bar_width; k++)
                {
                    if (x + k < 0 || (uint32_t)x + (uint32_t)k >= global->img->width)
                        continue;
                    int draw_y = top_y + j;
                    if (draw_y >= 0 && (uint32_t)draw_y < global->img->height)
                        mlx_put_pixel(global->img, x + k, draw_y, color);
                }
            }
        }
        i++;
    }
}
