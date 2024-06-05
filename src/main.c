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
	global.player = new_player((t_point){26, 11}, (t_point){0, 0});
	global.xfactor = abs(WIDTH / global.map->width);
    global.yfactor = abs(HEIGHT / global.map->height);
	mlx_loop_hook(global.mlx, loop, &global);
	mlx_loop(global.mlx);
	mlx_terminate(global.mlx);
	return (0);
}