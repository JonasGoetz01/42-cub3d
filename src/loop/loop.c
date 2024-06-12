#include "../../inc/cub3d.h"

void loop(void *param)
{
	t_global *global;

    global = NULL;
    global = (t_global *)param;
    mlx_delete_image(global->mlx, global->minimap);
    mlx_delete_image(global->mlx, global->img);
    global->minimap = mlx_new_image(global->mlx, global->window_width * global->minimap_scale, global->window_height * global->minimap_scale);
    global->img = mlx_new_image(global->mlx, global->window_width, global->window_height);
    make_background_transparent(global);
    showMap(global);
    show_sky_and_floor(global);
    render_3d(global);
    mlx_image_to_window(global->mlx, global->minimap, 0, 0);
    mlx_image_to_window(global->mlx, global->img, 0, 0);
    if(SHOW_MINIMAP)
    {
        // mlx_grow_instances(global->minimap, 0);
        mlx_image_to_window(global->mlx, global->minimap, 100, 100);
        if (global->minimap->count > 0)
            mlx_set_instance_depth(global->minimap->instances, 1);
    }
    // mlx_grow_instances(global->img, 0);
    mlx_image_to_window(global->mlx, global->img, 0, 0);
    if (global->img->count > 0)
        mlx_set_instance_depth(global->img->instances, 0);
}
