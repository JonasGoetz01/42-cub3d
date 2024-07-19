#include "../inc/cub3d.h"

void	init_to_null(t_global * global)
{
	global->map = NULL;
	global->player = NULL;
	global->doors = NULL;
	global->door_line = NULL;
	global->texture = NULL;
	global->lines = NULL;
}

void	ft_exit_free(t_global *global)
{
	if (global->map != NULL)
		ft_arr_free((void **)global->map);
	if (global->player != NULL)
	{
		if (global->player->rays != NULL)
			free(global->player->rays);
		free(global->player);
	}
	if (global->doors != NULL)
		free(global->doors);
	if (global->door_line != NULL)
		free(global->door_line);
	if (global->texture != NULL)
		free(global->texture);
	if (global->lines != NULL)
		free(global->lines);
	exit (EXIT_FAILURE);
}

int	main(int argc, char **argv)
{
	t_global global;

	init_to_null(&global);
	if (argc != 2)
		return (printf(ERR_ARG USAGE), EXIT_FAILURE);
	global.window_height = HEIGHT;
	global.window_width = WIDTH;
	global.minimap_scale = MINIMAP_SCALE;
	global.time = get_current_millis();
	global.open = false;
	global.close = false;
	if (parse_and_validate(argv[1], &global))
		ft_exit_free(&global);
	if (!(global.mlx = mlx_init(global.window_width, global.window_height,
				"cub3d", false)))
		ft_exit_free(&global);
	global.minimap = mlx_new_image(global.mlx, global.window_width,
			global.window_height);
	global.img = mlx_new_image(global.mlx, global.window_width,
			global.window_height);
	if (!global.img || !global.minimap)
		ft_exit_free(&global);
	global.free_mouse = false;
	global.sprite_index = 0;
	global.sprite_textures[0] = mlx_load_png("textures/gun1.png");
	global.sprite_textures[1] = mlx_load_png("textures/gun2.png");
	global.sprite_textures[2] = mlx_load_png("textures/gun3.png");
	global.sprite_textures[3] = mlx_load_png("textures/gun2.png");
	mlx_loop_hook(global.mlx, keyHook, &global);
	global.player = new_player(&global, (t_vec2d){get_player_position(&global).x
			* global.scale_factor, get_player_position(&global).y
			* global.scale_factor}, get_player_direction(&global));
	mlx_loop_hook(global.mlx, loop, &global);
	mlx_resize_hook(global.mlx, resize, &global);
	mlx_cursor_hook(global.mlx, cursor, &global);
	mlx_key_hook(global.mlx, key_hook, &global);
	mlx_loop(global.mlx);
	mlx_terminate(global.mlx);
	return (0);
}
