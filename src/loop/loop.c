#include "../../inc/cub3d.h"

void loop(void *param)
{
	t_global *global;

    global = NULL;
    global = (t_global *)param;
    mlx_delete_image(global->mlx, global->img);
    global->img = mlx_new_image(global->mlx, WIDTH, HEIGHT);
    showMap(global);
    mlx_image_to_window(global->mlx, global->img, 0, 0);
}
