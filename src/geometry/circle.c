#include "../../inc/cub3d.h"

void    circle(t_circle *circle, t_global *global, uint32_t color)
{
    int x = circle->radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (circle->radius << 1);

    while (x >= y)
    {
        mlx_put_pixel(global->img, circle->center->x + x, circle->center->y + y, color);
        mlx_put_pixel(global->img, circle->center->x + y, circle->center->y + x, color);
        mlx_put_pixel(global->img, circle->center->x - y, circle->center->y + x, color);
        mlx_put_pixel(global->img, circle->center->x - x, circle->center->y + y, color);
        mlx_put_pixel(global->img, circle->center->x - x, circle->center->y - y, color);
        mlx_put_pixel(global->img, circle->center->x - y, circle->center->y - x, color);
        mlx_put_pixel(global->img, circle->center->x + y, circle->center->y - x, color);
        mlx_put_pixel(global->img, circle->center->x + x, circle->center->y - y, color);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0)
        {
            x--;
            dx += 2;
            err += dx - (circle->radius << 1);
        }
    }
}

void    circle_point(int x, int y, int radius, t_global *global, uint32_t color)
{
    int x1 = radius - 1;
    int y1 = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x1 >= y1)
    {
        mlx_put_pixel(global->img, x + x1, y + y1, color);
        mlx_put_pixel(global->img, x + y1, y + x1, color);
        mlx_put_pixel(global->img, x - y1, y + x1, color);
        mlx_put_pixel(global->img, x - x1, y + y1, color);
        mlx_put_pixel(global->img, x - x1, y - y1, color);
        mlx_put_pixel(global->img, x - y1, y - x1, color);
        mlx_put_pixel(global->img, x + y1, y - x1, color);
        mlx_put_pixel(global->img, x + x1, y - y1, color);

        if (err <= 0)
        {
            y1++;
            err += dy;
            dy += 2;
        }
        if (err > 0)
        {
            x1--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}