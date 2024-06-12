#include "../../inc/cub3d.h"

void keyHook(void *param) {
    t_global *global;
    t_vec2d dir = {0.0f, 0.0f};  // Initialize the direction vector

    global = (t_global *)param;

    if (mlx_is_key_down(global->mlx, MLX_KEY_ESCAPE)) {
        mlx_close_window(global->mlx);
        return;
    }
    if (mlx_is_key_down(global->mlx, MLX_KEY_W)) {
        dir.x += global->player->dir.x;
        dir.y += global->player->dir.y;
    }
    if (mlx_is_key_down(global->mlx, MLX_KEY_S)) {
        dir.x -= global->player->dir.x;
        dir.y -= global->player->dir.y;
    }
    if (mlx_is_key_down(global->mlx, MLX_KEY_A)) {
        dir.x += global->player->dir.y;
        dir.y -= global->player->dir.x;
    }
    if (mlx_is_key_down(global->mlx, MLX_KEY_D)) {
        dir.x -= global->player->dir.y;
        dir.y += global->player->dir.x;
    }
    if (mlx_is_key_down(global->mlx, MLX_KEY_E)) {
        rotate_player(global, 0.03f);
    }
    if (mlx_is_key_down(global->mlx, MLX_KEY_Q)) {
        rotate_player(global, -0.03f);
    }
    float length = sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length != 0) {
        dir.x /= length;
        dir.y /= length;
    }
    update_position(global, dir);
}
