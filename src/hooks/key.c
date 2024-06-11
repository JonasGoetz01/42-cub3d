#include "../../inc/cub3d.h"

void    keyHook(mlx_key_data_t keydata, void* param)
{
    t_global *global = (t_global*)param;
    if(keydata.action == MLX_PRESS)
        return;
    if (keydata.key == MLX_KEY_ESCAPE)
    {
        mlx_close_window(global->mlx);
        return;
    }
    if (keydata.key == MLX_KEY_W)
    {
        global->player->pos.y -= MOVE_SPEED;
        update_position(global, global->player->pos);
    }
    if (keydata.key == MLX_KEY_S)
    {
        global->player->pos.y += MOVE_SPEED;
        update_position(global, global->player->pos);
    }
    if (keydata.key == MLX_KEY_A)
    {
        global->player->pos.x -= MOVE_SPEED;
        update_position(global, global->player->pos);
    }
    if (keydata.key == MLX_KEY_D)
    {
        global->player->pos.x += MOVE_SPEED;
        update_position(global, global->player->pos);
    }

    if (keydata.key == MLX_KEY_E)
    {
        rotate_player(global, 0.1f);
    }

    if (keydata.key == MLX_KEY_Q)
    {
        rotate_player(global, -0.1f);
    }
}
