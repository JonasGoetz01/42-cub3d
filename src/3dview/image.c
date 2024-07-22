/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:26:41 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 15:05:16 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	make_background_transparent(t_global *global)
{
	uint32_t	x;
	uint32_t	y;

	y = 0;
	while (y < global->minimap->height)
	{
		x = 0;
		while (x < global->minimap->width)
		{
			mlx_put_pixel(global->minimap, x, y, get_rgba(0, 0, 0, 100));
			x++;
		}
		y++;
	}
}

void	show_sky(t_global *global)
{
	int	x;
	int	y;

	y = -1;
	while (++y < global->window_height / 2)
	{
		x = 0;
		while (x < global->window_width)
		{
			mlx_put_pixel(global->img, x, y, get_rgba(global->ceiling.r,
					global->ceiling.g, global->ceiling.b, 255));
			x++;
		}
		y++;
	}
}

void	show_floor(t_global *global)
{
	int	x;
	int	y;

	y = global->window_height / 2;
	while (y < global->window_height)
	{
		x = 0;
		while (x < global->window_width)
		{
			mlx_put_pixel(global->img, x, y, get_rgba(global->floor.r,
					global->floor.g, global->floor.b, 255));
			x++;
		}
		y++;
	}
}

float	map_distance_to_height(float distance, t_global *global)
{
	float	height;
	int		window_height;

	window_height = global->window_height;
	if (distance == 0)
		height = window_height;
	else
		height = (global->scale_factor / distance) * window_height;
	return (height);
}

float	get_distance(t_vec2d a, t_vec2d b)
{
	return (sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2)));
}
