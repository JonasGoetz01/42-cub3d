/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:03:03 by jgotz             #+#    #+#             */
/*   Updated: 2024/07/22 10:10:02 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	get_rgba(int r, int g, int b, int a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

float	calculate_scale_factor(int map_width, int map_height, int window_width,
		int window_height)
{
	float	scale_x;
	float	scale_y;

	scale_x = ((float)window_width - 10) / (float)map_width;
	scale_y = ((float)window_height - 10) / (float)map_height;
	if (scale_x < scale_y)
		return (scale_x);
	return (scale_y);
}

void	scale_line_segments(t_line *lines, int line_count, float scale_factor)
{
	int	i;

	i = 0;
	while (i < line_count)
	{
		lines[i].a.x *= scale_factor;
		lines[i].a.y *= scale_factor;
		lines[i].b.x *= scale_factor;
		lines[i].b.y *= scale_factor;
		i++;
	}
}

void	draw_line_crosshair(t_global *global, t_vec2d a, t_vec2d b, int color)
{
	int		steps;
	float	x_inc;
	float	y_inc;
	float	x;
	float	y;

	if (abs((int)(b.x - a.x)) > abs((int)(b.y - a.y)))
		steps = abs((int)(b.x - a.x));
	else
		steps = abs((int)(b.y - a.y));
	x_inc = (int)(b.x - a.x) / (float)steps;
	y_inc = (int)(b.y - a.y) / (float)steps;
	x = a.x;
	y = a.y;
	a.x = 0;
	while ((int)a.x <= steps)
	{
		if (x >= 0 && x < global->img->width && y >= 0
			&& y < global->img->height)
			mlx_put_pixel(global->img, (int)x, (int)y, color);
		x += x_inc;
		y += y_inc;
		a.x++;
	}
}

void	draw_line(t_global *global, t_vec2d a, t_vec2d b, int color)
{
	int		steps;
	float	x_inc;
	float	y_inc;
	float	x;
	float	y;

	if (abs((int)(b.x - a.x)) > abs((int)(b.y - a.y)))
		steps = abs((int)(b.x - a.x));
	else
		steps = abs((int)(b.y - a.y));
	x_inc = (int)(b.x - a.x) / (float)steps;
	y_inc = (int)(b.y - a.y) / (float)steps;
	x = a.x;
	y = a.y;
	a.x = 0;
	while ((int)a.x <= steps)
	{
		if (x >= 0 && x < global->minimap->width && y >= 0
			&& y < global->minimap->height)
			mlx_put_pixel(global->minimap, (int)x, (int)y, color);
		x += x_inc;
		y += y_inc;
		a.x++;
	}
}
