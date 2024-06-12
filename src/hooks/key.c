#include "../../inc/cub3d.h"

void keyHook(void *param) {
    t_global *global;

    global = (t_global *)param;

    if (mlx_is_key_down(global->mlx, MLX_KEY_ESCAPE)) {
        mlx_close_window(global->mlx);
        return;
    }
    if (mlx_is_key_down(global->mlx, MLX_KEY_W)) {
        update_position(global, global->player->dir);
    }
    if (mlx_is_key_down(global->mlx, MLX_KEY_S)) {
        t_vec2d dir = {-global->player->dir.x, -global->player->dir.y};
        update_position(global, dir);
    }
    if (mlx_is_key_down(global->mlx, MLX_KEY_A)) {
        t_vec2d dir = {global->player->dir.y, -global->player->dir.x};
        update_position(global, dir);
    }
    if (mlx_is_key_down(global->mlx, MLX_KEY_D)) {
        t_vec2d dir = {-global->player->dir.y, global->player->dir.x};
        update_position(global, dir);
    }
    if (mlx_is_key_down(global->mlx, MLX_KEY_E)) {
        rotate_player(global, 0.1f);
    }
    if (mlx_is_key_down(global->mlx, MLX_KEY_Q)) {
        rotate_player(global, -0.1f);
    }
}

