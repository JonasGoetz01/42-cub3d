#include "../../inc/cub3d.h"

void	resize(int32_t width, int32_t height, void *param)
{
	t_global	*global;
	t_vec2d		player_pos;
	t_vec2d		player_dir;

	global = (t_global *)param;
	global->window_width = width;
	global->window_height = height;
	mlx_delete_image(global->mlx, global->minimap);
	if (!(global->minimap = mlx_new_image(global->mlx, (int)global->window_width
				* (int)global->minimap_scale, (int)global->window_height
				* (int)global->minimap_scale)))
	{
		mlx_close_window(global->mlx);
		printf("%s\n", mlx_strerror(mlx_errno));
		exit(EXIT_FAILURE);
	}
	global->scale_factor = calculate_scale_factor(global->map->width,
			global->map->height, (int)global->window_width
			* (int)global->minimap_scale, (int)global->window_height
			* (int)global->minimap_scale);
	map_to_line_segments(global, &global->lines, &global->line_count);
	scale_line_segments(global->lines, global->line_count,
		global->scale_factor);
	player_pos = get_player_position(global);
	player_pos.x *= global->scale_factor;
	player_pos.y *= global->scale_factor;
	player_dir = global->player->dir;
	free(global->player);
	global->player = new_player(global, player_pos, player_dir);
}
