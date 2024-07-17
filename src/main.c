#include "../inc/cub3d.h"

void key(mlx_key_data_t key, void *param)
{
	t_global *global;

	global = (t_global *)param;
	if (key.key == MLX_KEY_LEFT_CONTROL && key.action == MLX_PRESS)
	{
		global->open = true;
	}
	if (key.key == MLX_KEY_LEFT_CONTROL && key.action == MLX_RELEASE)
	{
		global->open = false;
	}
	if (key.key == MLX_KEY_RIGHT_CONTROL && key.action == MLX_PRESS)
	{
		global->close = true;
	}
	if (key.key == MLX_KEY_RIGHT_CONTROL && key.action == MLX_RELEASE)
	{
		global->close = false;
	}
}

int	main(int argc, char **argv)
{
	t_global global;
	// t_map map;

	if (argc != 2)
		return (printf(ERR_ARG USAGE), EXIT_FAILURE);
	global.window_height = HEIGHT;
	global.window_width = WIDTH;
	global.minimap_scale = MINIMAP_SCALE;
	global.time = get_current_millis();
	global.open = false;
	global.close = false;
	if (parse_and_validate(argv[1], &global))
	{
		//call cleanup function (not implemented yet)
		return (EXIT_FAILURE);
	}
	mlx_texture_t *logo = mlx_load_png("textures/wolfenstein-logo.png");
	if (!logo)
	{
		printf("Failed to load logo\n");
		printf("%s\n", mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	if (!(global.mlx = mlx_init(global.window_width, global.window_height,
				"cub3d", true)))
	{
		printf("%s\n", mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	mlx_set_icon(global.mlx, logo);
	if (!(global.minimap = mlx_new_image(global.mlx, global.window_width,
				global.window_height)))
	{
		mlx_close_window(global.mlx);
		printf("%s\n", mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	if (!(global.img = mlx_new_image(global.mlx, global.window_width,
				global.window_height)))
	{
		mlx_close_window(global.mlx);
		printf("%s\n", mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	// global.map = &map;
	mlx_loop_hook(global.mlx, keyHook, &global);
	// initMap(&global);
	global.player = new_player(&global, (t_vec2d){get_player_position(&global).x
			* global.scale_factor, get_player_position(&global).y
			* global.scale_factor}, get_player_direction(&global));
	mlx_loop_hook(global.mlx, loop, &global);
	mlx_key_hook(global.mlx, key, &global);
	mlx_resize_hook(global.mlx, resize, &global);
	mlx_cursor_hook(global.mlx, cursor, &global);
	mlx_set_cursor_mode(global.mlx, MLX_MOUSE_HIDDEN);
	mlx_loop(global.mlx);
	mlx_terminate(global.mlx);
	return (0);
}
