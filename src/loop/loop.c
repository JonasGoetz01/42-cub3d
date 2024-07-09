#include "../../inc/cub3d.h"

void	loop(void *param)
{
	t_global	*global;
	static int	fps_timer = 0;
	double		current_time;
	double		elapsed_time;
	int			fps;

	global = (t_global *)param;
	current_time = get_current_millis();
	elapsed_time = current_time - global->time;
	global->time = current_time;
	if (fps_timer % 10 == 0)
	{
		fps = 1000 / elapsed_time;
		printf("\rFPS: %d", fps);
		fflush(stdout);
		fps_timer = 0;
	}
	fps_timer++;
	mlx_delete_image(global->mlx, global->minimap);
	mlx_delete_image(global->mlx, global->img);
	global->minimap = mlx_new_image(global->mlx, global->window_width
			* global->minimap_scale, global->window_height
			* global->minimap_scale);
	global->img = mlx_new_image(global->mlx, global->window_width,
			global->window_height);
	make_background_transparent(global);
	showMap(global);
	show_sky_and_floor(global);
	render_3d(global);
	mlx_image_to_window(global->mlx, global->minimap, 0, 0);
	mlx_image_to_window(global->mlx, global->img, 0, 0);
	if (SHOW_MINIMAP)
	{
		mlx_image_to_window(global->mlx, global->minimap, 100, 100);
		if (global->minimap->count > 0)
			mlx_set_instance_depth(global->minimap->instances, 1);
	}
	mlx_image_to_window(global->mlx, global->img, 0, 0);
	if (global->img->count > 0)
		mlx_set_instance_depth(global->img->instances, 0);
}
