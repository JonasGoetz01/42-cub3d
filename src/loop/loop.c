#include "../../inc/cub3d.h"

void	ft_texture_to_image(t_global *global, mlx_texture_t *texture)
{
	int		i;
	int		i_start;
	int		j;
	int		j_start;
	int		color;
	uint8_t	*pixel;

	i = (int)global->window_height - (int)texture->height;
	i_start = i;
	while (i < global->window_height)
	{
		j = (int)global->window_width - (int)texture->width;
		j_start = j;
		while (j < global->window_width)
		{
			pixel = &(texture->pixels[((i - i_start) * texture->width + (j
							- j_start)) * texture->bytes_per_pixel]);
			if (pixel != NULL)
			{
				color = get_rgba(pixel[0], pixel[1], pixel[2], 255);
				if (pixel[3] != 0)
				{
					if ((uint32_t)i < global->img->height
						&& (uint32_t)j < global->img->width)
					{
						mlx_put_pixel(global->img, j, i, color);
					}
				}
			}
			j++;
		}
		i++;
	}
}

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
		fps = (int)((double)1000.0 / elapsed_time);
		printf("\rFPS: %d", fps);
		fflush(stdout);
		fps_timer = 0;
	}
	fps_timer++;
	update_door_segments(global);
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
	ft_texture_to_image(global, global->sprite_textures[global->sprite_index]);
	t_line crosshair[2];
	crosshair[0].a = (t_vec2d){global->window_width / 2 - 10,
		global->window_height / 2};
	crosshair[0].b = (t_vec2d){global->window_width / 2 + 10,
		global->window_height / 2};
	crosshair[1].a = (t_vec2d){global->window_width / 2,
		global->window_height / 2 - 10};
	crosshair[1].b = (t_vec2d){global->window_width / 2,
		global->window_height / 2 + 10};
	draw_line_crosshair(global, crosshair[0].a, crosshair[0].b, 0x00FF00);
	draw_line_crosshair(global, crosshair[1].a, crosshair[1].b, 0x00FF00);
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
