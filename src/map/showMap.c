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
    
    global->xfactor = abs(WIDTH / global->map->width);
    global->yfactor = abs(HEIGHT / global->map->height);
    if (global->xfactor > global->yfactor)
        global->xfactor = global->yfactor;
    else
        global->yfactor = global->xfactor;

    // t_point player_pos = (t_point){26 * xfactor, 11 * yfactor};
    // t_circle player = (t_circle){&player_pos, 5};
    
    for (int y = 0; y < global->map->height; y++)
    {
        for (int x = 0; x < global->map->width; x++)
        {
            if (global->map->map[y][x] == '1')
            {
                fillRect(global, x * global->xfactor, y * global->yfactor, global->xfactor, global->yfactor, get_rgba(255, 255, 255, 255));
                if (!isWallAbove(global, x, y))
                    line((t_point){x * global->xfactor, y * global->yfactor}, (t_point){(x + 1) * global->xfactor, y * global->yfactor}, get_rgba(255, 255, 255, 255), global);
                if (!isWallBelow(global, x, y))
                    line((t_point){x * global->xfactor, (y + 1) * global->yfactor}, (t_point){(x + 1) * global->xfactor, (y + 1) * global->yfactor}, get_rgba(255, 255, 255, 255), global);
                if (!isWallLeft(global, x, y))
                    line((t_point){x * global->xfactor, y * global->yfactor}, (t_point){x * global->xfactor, (y + 1) * global->yfactor}, get_rgba(255, 255, 255, 255), global);
                if (!isWallRight(global, x, y))
                    line((t_point){(x + 1) * global->xfactor, y * global->yfactor}, (t_point){(x + 1) * global->xfactor, (y + 1) * global->yfactor}, get_rgba(255, 255, 255, 255), global);
            }
            circle_point(global->player->pos.x * global->xfactor, global->player->pos.y * global->yfactor, 5, global, get_rgba(255, 0, 0, 255));
            render_rays(global);
        }
    }
    // printf("xfactor: %d\n", xfactor);
    // printf("yfactor: %d\n", yfactor);
}
