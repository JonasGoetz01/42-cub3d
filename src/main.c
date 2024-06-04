#include "../inc/cub3d.h"

int	main(int argc, char **argv)
{
	mlx_t		*mlx;
	mlx_image_t	*img;

	(void)argc;
    (void)argv;
	mlx = mlx_init(WIDTH, HEIGHT, "cub3d", true);
	img = mlx_new_image(mlx, WIDTH, HEIGHT);
	// mlx_key_hook(mlx, quit, &mbt);
	// mlx_scroll_hook(mlx, scroll, &mbt);
	// mlx_resize_hook(mlx, resize, &mbt);
	// mlx_cursor_hook(mlx, mouse_event, &mbt);
	// mlx_mouse_hook(mlx, mouse_click_event, &mbt);
	mlx_loop(mlx);
	return (0);
}