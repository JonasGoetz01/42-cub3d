#include "../inc/cub3d.h"

int	main(int argc, char **argv)
{
	t_global global;
	t_map map;

	(void)argc;
    (void)argv;
	global.window_height = HEIGHT;
	global.window_width = WIDTH;
	global.minimap_scale = MINIMAP_SCALE;
	if (!(global.mlx = mlx_init(global.window_width, global.window_height, "cub3d", true)))
	{
		printf("%s\n", mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (!(global.minimap = mlx_new_image(global.mlx, global.window_width, global.window_height)))
	{
		mlx_close_window(global.mlx);
		printf("%s\n", mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (!(global.img = mlx_new_image(global.mlx, global.window_width, global.window_height)))
	{
		mlx_close_window(global.mlx);
		printf("%s\n", mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	global.map = &map;
	mlx_key_hook(global.mlx, keyHook, &global);
	initMap(&global);
	global.player = new_player((t_vec2d){26 * global.scale_factor, 11 * global.scale_factor}, (t_vec2d){0, -1});
	mlx_loop_hook(global.mlx, loop, &global);
	mlx_resize_hook(global.mlx, resize, &global);
	mlx_cursor_hook(global.mlx, cursor, &global);
    mlx_set_cursor_mode(global.mlx, MLX_MOUSE_HIDDEN);
	mlx_loop(global.mlx);
	mlx_terminate(global.mlx);
	return (0);
}