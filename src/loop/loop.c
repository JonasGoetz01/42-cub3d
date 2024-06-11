#include "../../inc/cub3d.h"

void loop(void *param)
{
	t_global *global;

    global = NULL;
    global = (t_global *)param;
    mlx_delete_image(global->mlx, global->minimap);
    global->minimap = mlx_new_image(global->mlx, global->window_width, global->window_height);
    showMap(global);
    mlx_image_to_window(global->mlx, global->minimap, 0, 0);
}
