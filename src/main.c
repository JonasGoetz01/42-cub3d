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
	// mlx_key_hook(mlx, quit, &mbt);
	// mlx_scroll_hook(mlx, scroll, &mbt);
	// mlx_resize_hook(mlx, resize, &mbt);
	// mlx_cursor_hook(mlx, mouse_event, &mbt);
	// mlx_mouse_hook(mlx, mouse_click_event, &mbt);
	initMap(&global);
	mlx_loop_hook(global.mlx, loop, &global);
	mlx_loop(global.mlx);
	return (0);
}