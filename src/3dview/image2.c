/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:26:41 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 13:38:21 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	render_3d(t_global *global)
{
	int		i;
	float	player_angle;
	float	z_buffer[WIDTH];

	player_angle = atan2(global->player->dir.y, global->player->dir.x);
	if (global->close)
		check_close_door(global);
	if (global->open)
		check_open_door(global);
	i = 0;
	while (i < (int)global->img->width)
	{
		process_ray(global, i, player_angle, z_buffer);
		i++;
	}
}

void	process_ray(t_global *global, int i, float player_angle,
		float *z_buffer)
{
	t_ray			*ray;
	t_collision		*closest_collision;
	int				bar_height;
	float			hit_percentage;
	mlx_texture_t	*texture;

	ray = &global->player->rays[i];
	closest_collision = ray->closest_collision;
	if (closest_collision)
	{
		bar_height = map_distance_to_height(fmaxf(get_distance(
						global->player->pos, closest_collision->point), 0.1f)
				* cosf(atan2f(ray->direction.y, ray->direction.x)
					- player_angle), global);
		texture = load_texture(global, closest_collision);
		hit_percentage = calculate_hit_percentage(closest_collision);
		hit_percentage = fmax(fmin(hit_percentage, 1.0f), 0.0f);
		draw_texture_column(global, (t_vec2d){(int)i, (int)(global->img->height
				/ 2 - (bar_height / 2))}, texture,
			(t_vec2d){(int)((hit_percentage * (texture->width))), bar_height});
		z_buffer[i] = (float)fmaxf(get_distance(global->player->pos,
					closest_collision->point), 0.1f)
			* cosf(atan2f(ray->direction.y, ray->direction.x) - player_angle);
	}
}
