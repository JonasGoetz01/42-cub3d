/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:03:03 by jgotz             #+#    #+#             */
/*   Updated: 2024/07/15 11:20:45 by jgotz            ###   ########.fr       */
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
	return ((scale_x < scale_y) ? scale_x : scale_y);
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
	b.x = 0;
	while (b.x <= steps)
	{
		if ((uint32_t)x >= 0 && (uint32_t)x < global->minimap->width
			&& (uint32_t)y >= 0 && (uint32_t)y < global->minimap->height)
		{
			mlx_put_pixel(global->minimap, (int)x, (int)y, color);
		}
		x += x_inc;
		y += y_inc;
		b.x++;
	}
}
void	put_circle_pixels(t_global *global, t_circle *circle, int x, int y,
		int color)
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

void	init_values_draw_circle(int *x, int *y, int *dx, t_circle *circle)
{
	*x = (int)circle->radius - 1;
	*y = 0;
	*dx = 1;
}

void	draw_circle(t_global *global, t_circle *circle, int color)
{
	int	x;
	int	y;
	int	dx;
	int	dy;
	int	err;

	init_values_draw_circle(&x, &y, &dx, circle);
	dy = 1;
	err = dx - ((int)circle->radius << 1);
	while (x >= y)
	{
		put_circle_pixels(global, circle, x, y, color);
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
			err += dx - ((int)circle->radius << 1);
		}
	}
}

void	draw_ray(t_global *global, t_ray *ray)
{
	t_vec2d	end;

	end = (t_vec2d){ray->origin.x + ray->direction.x * 1000, ray->origin.y
		+ ray->direction.y * 1000};
	draw_line(global, ray->origin, end, get_rgba(255, 255, 255, 255));
}

t_vec2d	ray_line_collision(t_ray *ray, t_line *line, t_face *face)
{
	float	x1;
	float	y1;
	float	x2;
	float	y2;
	float	x3;
	float	y3;
	float	x4;
	float	y4;
	float	denominator;
	float	t;
	float	u;
	t_vec2d	collision_point;

	x1 = ray->origin.x;
	y1 = ray->origin.y;
	x2 = ray->origin.x + ray->direction.x;
	y2 = ray->origin.y + ray->direction.y;
	x3 = line->a.x;
	y3 = line->a.y;
	x4 = line->b.x;
	y4 = line->b.y;
	denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	if (denominator == 0)
		return ((t_vec2d){-1, -1});
	t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denominator;
	u = ((x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2)) / denominator;
	if (t >= 0 && u >= 0 && u <= 1)
	{
		collision_point = (t_vec2d){x1 + t * (x2 - x1), y1 + t * (y2 - y1)};
		if (line->alignment == HORIZONTAL)
		{
			if (ray->origin.y < collision_point.y)
				*face = SOUTH;
			else
				*face = NORTH;
		}
		else // VERTICAL
		{
			if (ray->origin.x < collision_point.x)
				*face = EAST;
			else
				*face = WEST;
		}
		return (collision_point);
	}
	return ((t_vec2d){-1, -1});
}

t_collision	*new_collision(t_collision *collisions, int *collision_count,
		t_vec2d point, t_line *line, t_face face)
{
	t_collision	*new_collisions;

	new_collisions = realloc(collisions, (*collision_count + 1)
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

	for (int i = 0; i < (int)global->img->width; i++)
	{
		for (int j = 0; j < global->line_count; j++)
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
		}
		min_distance = 1000000;
		for (int j = 0; j < global->player->rays[i].collision_count; j++)
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
		}
	}
}

// draw a bar from the center of the screen "width" pixels wide and "height" pixels tall
void	draw_bar(t_global *global, int x, int y, int width, int height,
		int color)
{
	int	draw_y;

	for (int i = 0; i < width; i++)
	{
		if (x + i < 0 || (uint32_t)x + (uint32_t)i >= global->img->width)
			continue ;
		for (int j = 0; j < height; j++)
		{
			draw_y = y + j;
			if (draw_y >= 0 && (uint32_t)draw_y < global->img->height)
			{
				mlx_put_pixel(global->img, x + i, draw_y, color);
			}
		}
	}
}
