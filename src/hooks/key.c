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
        t_vec2d new_pos = {global->player->pos.x + global->player->dir.x * MOVE_SPEED * global->minimap_scale, global->player->pos.y + global->player->dir.y * MOVE_SPEED * global->minimap_scale};
        update_position(global, new_pos);
    }
    if (keydata.key == MLX_KEY_S)
    {
        t_vec2d new_pos = {global->player->pos.x - global->player->dir.x * MOVE_SPEED * global->minimap_scale, global->player->pos.y - global->player->dir.y * MOVE_SPEED * global->minimap_scale};
        update_position(global, new_pos);
    }
    if (keydata.key == MLX_KEY_D)
    {
        t_vec2d new_pos = {global->player->pos.x - global->player->dir.y * MOVE_SPEED * global->minimap_scale, global->player->pos.y + global->player->dir.x * MOVE_SPEED * global->minimap_scale};
        update_position(global, new_pos);
    }
    if (keydata.key == MLX_KEY_A)
    {
        t_vec2d new_pos = {global->player->pos.x + global->player->dir.y * MOVE_SPEED * global->minimap_scale, global->player->pos.y - global->player->dir.x * MOVE_SPEED * global->minimap_scale};
        update_position(global, new_pos);
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
