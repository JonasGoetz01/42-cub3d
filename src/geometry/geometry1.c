/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:03:03 by jgotz             #+#    #+#             */
/*   Updated: 2024/07/22 10:10:01 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	put_pixel_if_in_bounds(t_global *global, int x, int y, int color)
{
	if (x >= 0 && x < (int)global->window_width && y >= 0
		&& y < (int)global->window_height)
		mlx_put_pixel(global->minimap, x, y, color);
}

void	plot_circle_points(t_global *global, t_circle *circle, int color,
		t_vec2d xy)
{
	int	cx;
	int	cy;

	cx = (int)circle->center.x;
	cy = (int)circle->center.y;
	put_pixel_if_in_bounds(global, cx + xy.x, cy + xy.y, color);
	put_pixel_if_in_bounds(global, cx + xy.y, cy + xy.x, color);
	put_pixel_if_in_bounds(global, cx - xy.y, cy + xy.x, color);
	put_pixel_if_in_bounds(global, cx - xy.x, cy + xy.y, color);
	put_pixel_if_in_bounds(global, cx - xy.x, cy - xy.y, color);
	put_pixel_if_in_bounds(global, cx - xy.y, cy - xy.x, color);
	put_pixel_if_in_bounds(global, cx + xy.y, cy - xy.x, color);
	put_pixel_if_in_bounds(global, cx + xy.x, cy - xy.y, color);
}

void	draw_circle_points(t_global *global, t_circle *circle, int color,
		int radius)
{
	t_vec2d	xy;
	t_vec2d	deltas;
	int		err;

	xy.x = radius - 1;
	xy.y = 0;
	deltas.x = 1;
	deltas.y = 1;
	err = deltas.x - (radius << 1);
	while (xy.x >= xy.y)
	{
		plot_circle_points(global, circle, color, (t_vec2d){xy.x, xy.y});
		if (err <= 0)
		{
			xy.y++;
			err += deltas.y;
			deltas.y += 2;
		}
		if (err > 0)
		{
			xy.x--;
			deltas.x += 2;
			err += deltas.x - (radius << 1);
		}
	}
}

void	draw_circle(t_global *global, t_circle *circle, int color)
{
	int	radius;

	radius = (int)circle->radius;
	draw_circle_points(global, circle, color, radius);
}

void	draw_ray(t_global *global, t_ray *ray)
{
	t_vec2d	end;

	end = (t_vec2d){ray->origin.x + ray->direction.x * 1000, ray->origin.y
		+ ray->direction.y * 1000};
	draw_line(global, ray->origin, end, get_rgba(255, 255, 255, 255));
}
