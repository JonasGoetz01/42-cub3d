#include "../../inc/cub3d.h"

bool isWallAbove(t_global *global, int x, int y)
{
    if (y == 0)
        return (false);
    if (global->map->map[y - 1][x] == '1')
        return (true);
    return (false);
}

bool isWallBelow(t_global *global, int x, int y)
{
    if (y == global->map->height - 1)
        return (false);
    if (global->map->map[y + 1][x] == '1')
        return (true);
    return (false);
}

bool isWallLeft(t_global *global, int x, int y)
{
    if (x == 0)
        return (false);
    if (global->map->map[y][x - 1] == '1')
        return (true);
    return (false);
}

bool isWallRight(t_global *global, int x, int y)
{
    if (x == global->map->width - 1)
        return (false);
    if (global->map->map[y][x + 1] == '1')
        return (true);
    return (false);
}

void fillRect(t_global *global, int x, int y, int width, int height, int color)
{
    for (int i = x; i < x + width; i++)
    {
        for (int j = y; j < y + height; j++)
        {
            mlx_put_pixel(global->img, i, j, color);
        }
    }
}


void showMap(t_global *global)
{
    int xfactor = abs(WIDTH / global->map->width);
    int yfactor = abs(HEIGHT / global->map->height);
    if (xfactor > yfactor)
        xfactor = yfactor;
    else
        yfactor = xfactor;

    for (int y = 0; y < global->map->height; y++)
    {
        for (int x = 0; x < global->map->width; x++)
        {
            if (global->map->map[y][x] == '1')
            {
                fillRect(global, x * xfactor, y * yfactor, xfactor, yfactor, get_rgba(255, 255, 255, 255));
                if (!isWallAbove(global, x, y))
                    line((t_point){x * xfactor, y * yfactor}, (t_point){(x + 1) * xfactor, y * yfactor}, get_rgba(255, 255, 255, 255), global);
                if (!isWallBelow(global, x, y))
                    line((t_point){x * xfactor, (y + 1) * yfactor}, (t_point){(x + 1) * xfactor, (y + 1) * yfactor}, get_rgba(255, 255, 255, 255), global);
                if (!isWallLeft(global, x, y))
                    line((t_point){x * xfactor, y * yfactor}, (t_point){x * xfactor, (y + 1) * yfactor}, get_rgba(255, 255, 255, 255), global);
                if (!isWallRight(global, x, y))
                    line((t_point){(x + 1) * xfactor, y * yfactor}, (t_point){(x + 1) * xfactor, (y + 1) * yfactor}, get_rgba(255, 255, 255, 255), global);
            }
        }
    }
}
