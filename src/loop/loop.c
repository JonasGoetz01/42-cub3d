/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:03:03 by jgotz             #+#    #+#             */
/*   Updated: 2024/07/22 15:08:34 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	draw_crosshair(t_global *global)
{
	t_line	crosshair[2];

	crosshair[0].a = (t_vec2d){global->window_width / 2 - 10,
		global->window_height / 2};
	crosshair[0].b = (t_vec2d){global->window_width / 2 + 10,
		global->window_height / 2};
	crosshair[1].a = (t_vec2d){global->window_width / 2, global->window_height
		/ 2 - 10};
	crosshair[1].b = (t_vec2d){global->window_width / 2, global->window_height
		/ 2 + 10};
	draw_line_crosshair(global, crosshair[0].a, crosshair[0].b, 0x00FF00);
	draw_line_crosshair(global, crosshair[1].a, crosshair[1].b, 0x00FF00);
}

void	update_graphics(t_global *global)
{
	update_door_lines(global);
	update_images(global);
	make_background_transparent(global);
	show_map(global);
	show_sky(global);
	show_floor(global);
	render_3d(global);
	ft_texture_to_image(global, global->sprite_textures[global->sprite_index]);
	draw_crosshair(global);
	mlx_image_to_window(global->mlx, global->minimap, 0, 0);
	mlx_image_to_window(global->mlx, global->img, 0, 0);
}

void	display_minimap(t_global *global)
{
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

void	loop(void *param)
{
	t_global		*global;
	double			current_time;
	double			elapsed_time;
	const double	max_fps = 60.0;

	global = (t_global *)param;
	current_time = get_current_millis();
	elapsed_time = current_time - global->time;
	global->time = current_time;
	cap_fps(&elapsed_time, (1000.0 / max_fps), global);
	update_graphics(global);
	display_minimap(global);
}
