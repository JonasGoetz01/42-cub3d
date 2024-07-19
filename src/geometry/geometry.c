/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:03:03 by jgotz             #+#    #+#             */
/*   Updated: 2024/07/19 14:06:04 by jgotz            ###   ########.fr       */
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

void	plot_circle_points(t_global *global, t_circle *circle, int color, int x,
		int y)
{
	int	img_width;
	int	img_height;
	int	cx;
	int	cy;

	img_width = (int)global->window_width;
	img_height = (int)global->window_height;
	cx = (int)circle->center.x;
	cy = (int)circle->center.y;
	if (cx + x >= 0 && cx + x < img_width && cy + y >= 0 && cy + y < img_height)
		mlx_put_pixel(global->minimap, cx + x, cy + y, color);
	if (cx + y >= 0 && cx + y < img_width && cy + x >= 0 && cy + x < img_height)
		mlx_put_pixel(global->minimap, cx + y, cy + x, color);
	if (cx - y >= 0 && cx - y < img_width && cy + x >= 0 && cy + x < img_height)
		mlx_put_pixel(global->minimap, cx - y, cy + x, color);
	if (cx - x >= 0 && cx - x < img_width && cy + y >= 0 && cy + y < img_height)
		mlx_put_pixel(global->minimap, cx - x, cy + y, color);
	if (cx - x >= 0 && cx - x < img_width && cy - y >= 0 && cy - y < img_height)
		mlx_put_pixel(global->minimap, cx - x, cy - y, color);
	if (cx - y >= 0 && cx - y < img_width && cy - x >= 0 && cy - x < img_height)
		mlx_put_pixel(global->minimap, cx - y, cy - x, color);
	if (cx + y >= 0 && cx + y < img_width && cy - x >= 0 && cy - x < img_height)
		mlx_put_pixel(global->minimap, cx + y, cy - x, color);
	if (cx + x >= 0 && cx + x < img_width && cy - y >= 0 && cy - y < img_height)
		mlx_put_pixel(global->minimap, cx + x, cy - y, color);
}

void	draw_circle_points(t_global *global, t_circle *circle, int color,
		int radius)
{
	int	x;
	int	y;
	int	dx;
	int	dy;
	int	err;

	x = radius - 1;
	y = 0;
	dx = 1;
	dy = 1;
	err = dx - (radius << 1);
	while (x >= y)
	{
		plot_circle_points(global, circle, color, x, y);
		if (err <= 0)
		{
			y++;
			err += dy;
			dy += 2;
		}
		if (err > 0)
		{
			x--;
			dx += 2;
			err += dx - (radius << 1);
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

double	point_line_distance(t_vec2d point, t_line *line)
{
	double	dx;
	double	dy;
	double	t;
	double	closestX;
	double	closestY;

	dx = line->b.x - line->a.x;
	dy = line->b.y - line->a.y;
	if (dx == 0 && dy == 0)
	{
		dx = point.x - line->a.x;
		dy = point.y - line->a.y;
		return (sqrt(dx * dx + dy * dy));
	}
	t = ((point.x - line->a.x) * dx + (point.y - line->a.y) * dy) / (dx * dx
			+ dy * dy);
	t = fmax(0, fmin(1, t));
	closestX = line->a.x + t * dx;
	closestY = line->a.y + t * dy;
	dx = point.x - closestX;
	dy = point.y - closestY;
	return (sqrt(dx * dx + dy * dy));
}

t_vec2d	calculate_collision_point(float t, t_vec2d ray_origin,
		t_vec2d ray_direction)
{
	return ((t_vec2d){ray_origin.x + t * (ray_direction.x), ray_origin.y + t
		* (ray_direction.y)});
}

		if (line->alignment == HORIZONTAL)
		{
			if (line->type == WALL)
			{
				if (ray->origin.y < collision_point.y)
					*face = SOUTH;
				else
					*face = NORTH;
			}
			else
				*face = DOORS;
		}
		else // VERTICAL
		{
			if (line->type == WALL)
			{
				if (ray->origin.x < collision_point.x)
					*face = EAST;
				else
					*face = WEST;
			}
			else
				*face = DOORS;
		}
		return (collision_point);
	}
	return ((t_vec2d){-1, -1});
}

t_collision	*new_collision(t_collision *collisions, int *collision_count,
		t_vec2d point, t_line *line, t_face face)
{
	t_collision	*new_collisions;

	new_collisions = ft_realloc(collisions, *collision_count
			* sizeof(t_collision), (*collision_count + 1)
			* sizeof(t_collision));
	if (!new_collisions)
		return (NULL);
	new_collisions[*collision_count].point = point;
	new_collisions[*collision_count].line = line;
	new_collisions[*collision_count].face = face;
	(*collision_count)++;
	return (new_collisions);
}

void	raycast(t_global *global)
{
	t_vec2d		intersection;
	t_face		face;
	t_collision	*new_collisions;
	float		min_distance;
	float		distance;
	int			i;
	int			j;

	i = 0;
	while (i < (int)global->img->width)
	{
		j = 0;
		while (j < global->line_count)
		{
			intersection = ray_line_collision(&global->player->rays[i],
					&global->lines[j], &face);
			if (intersection.x != -1)
			{
				new_collisions = new_collision(global->player->rays[i].collisions,
						&global->player->rays[i].collision_count, intersection,
						&global->lines[j], face);
				if (!new_collisions)
					return ;
				global->player->rays[i].collisions = new_collisions;
			}
			j++;
		}
		min_distance = 1000000;
		j = 0;
		while (j < global->player->rays[i].collision_count)
		{
			distance = sqrtf(powf(global->player->pos.x
						- global->player->rays[i].collisions[j].point.x, 2)
					+ powf(global->player->pos.y
						- global->player->rays[i].collisions[j].point.y, 2));
			if (distance < min_distance)
			{
				min_distance = distance;
				global->player->rays[i].closest_collision = &global->player->rays[i].collisions[j];
			}
			j++;
		}
		i++;
	}
}

void	draw_bar(t_global *global, int x, int y, int width, int height,
		int color)
{
	int draw_y;
	int i;
	int j;

	i = 0;
	while (i < width)
	{
		if (x + i < 0 || (uint32_t)x + (uint32_t)i >= global->img->width)
			continue ;
		j = 0;
		while (j < height)
		{
			draw_y = y + j;
			if (draw_y >= 0 && (uint32_t)draw_y < global->img->height)
			{
				mlx_put_pixel(global->img, x + i, draw_y, color);
			}
			j++;
		}
		i++;
	}
}