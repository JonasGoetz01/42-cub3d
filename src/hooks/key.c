#include "../../inc/cub3d.h"


void    keyHook(mlx_key_data_t keydata, void* param)
{
    t_global *global = (t_global*)param;
    if (keydata.key == MLX_KEY_ESCAPE)
    {
        mlx_close_window(global->mlx);
        return;
    }
    else if (keydata.key == MLX_KEY_W)
    {
        global->player->pos.y -= 1;
    }
    else if (keydata.key == MLX_KEY_S)
    {
        global->player->pos.y += 1;
    }
    else if (keydata.key == MLX_KEY_A)
    {
        global->player->pos.x -= 1;
    }
    else if (keydata.key == MLX_KEY_D)
    {
        global->player->pos.x += 1;
    }
    else if (keydata.key == MLX_KEY_LEFT)
    {
        global->player->dir.x -= 1;
    }
    else if (keydata.key == MLX_KEY_RIGHT)
    {
        global->player->dir.x += 1;
    }
    else if (keydata.key == MLX_KEY_UP)
    {
        global->player->dir.y -= 1;
    }
    else if (keydata.key == MLX_KEY_DOWN)
    {
        global->player->dir.y += 1;
    }
}