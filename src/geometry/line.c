#include "../../inc/cub3d.h"

void    line(t_point a, t_point b, uint32_t color, t_global *global)
{
    int sy;
    int sx;
    int dx;
    int dy;
    int err;

    dx = abs(b.x - a.x);
    dy = -abs(b.y - a.y);
    if (a.y < b.y)
        sy = 1;
    else
        sy = -1;
    if (a.x < b.x)
        sx = 1;
    else
        sx = -1;
    err = dx + dy;
    while (1)
    {
        mlx_put_pixel(global->img, a.x, a.y, color);
        if (a.x == b.x && a.y == b.y)
            break;
        int e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            a.x += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            a.y += sy;
        }
    }
}

