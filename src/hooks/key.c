#include "../../inc/cub3d.h"


void    keyHook(mlx_key_data_t keydata, void* param)
{
    t_global *global = (t_global*)param;
    if (keydata.key == MLX_KEY_ESCAPE)
    {
        mlx_close_window(global->mlx);
        return;
    }
}