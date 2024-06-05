#include "../../inc/cub3d.h"

void    line(t_point a, t_point b, uint32_t color, t_global *global)
{
    int sy;
    int sx;
    double dx;
    double dy;
    int err;

    dx = fabs(b.x - a.x);
    dy = -fabs(b.y - a.y);
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
        if (a.x >= 0 && a.x < WIDTH && a.y >= 0 && a.y < HEIGHT)
            mlx_put_pixel(global->img, (int)a.x, (int)a.y, color);
        else 
            break;
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

//print a line from a with a certain angle till the end of the screen
void    line_angle(t_point a, double angle, uint32_t color, t_global *global)
{
    t_point b;

    b.x = a.x + cos(angle * M_PI / 180) * WIDTH;
    b.y = a.y + sin(angle * M_PI / 180) * HEIGHT;
    line(a, b, color, global);
}

