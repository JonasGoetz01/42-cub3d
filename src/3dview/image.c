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
            mlx_put_pixel(global->minimap, x, y, get_rgba(0, 0, 0, 0));
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