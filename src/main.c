#include "../inc/cub3d.h"

int	main(int argc, char **argv)
{
	t_global global;
	t_map map;

	(void)argc;
    (void)argv;
	if (!(global.mlx = mlx_init(WIDTH, HEIGHT, "cub3d", true)))
	{
		printf("%s\n", mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (!(global.img = mlx_new_image(global.mlx, WIDTH, HEIGHT)))
	{
		mlx_close_window(global.mlx);
		printf("%s\n", mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	global.map = &map;
	mlx_key_hook(global.mlx, keyHook, &global);
	initMap(&global);
	global.player = (t_player *)malloc(sizeof(t_player));
	if (!global.player)
	{
		mlx_close_window(global.mlx);
		printf("Error\n");
		return (EXIT_FAILURE);
	}
	global.player->pos = (t_vec2d){26, 11};
	global.player->dir = (t_vec2d){0, 1};
	mlx_loop_hook(global.mlx, loop, &global);
	mlx_loop(global.mlx);
	mlx_terminate(global.mlx);
	return (0);
}