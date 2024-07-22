/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:26:41 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 12:13:05 by jgotz            ###   ########.fr       */
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

	y = 0;
	while (y < global->window_height / 2)
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

double	point_line_distance(t_vec2d point, t_line *line);

void	check_inactive_lines(t_global *global)
{
	float		distance;
	t_ray		tmp_ray;
	t_collision	*tmp;
	t_vec2d		intersection;
	t_face		face;
	float		min_distance;

	min_distance = 1000000;
	tmp_ray.origin = global->player->pos;
	tmp_ray.direction = global->player->dir;
	tmp_ray.collisions = NULL;
	tmp_ray.collision_count = 0;
	for (int i = 0; i < global->line_count; i++)
	{
		intersection = ray_line_collision(&tmp_ray, &global->lines[i], &face);
		if (intersection.x != -1)
		{
			tmp = new_collision(tmp_ray.collisions, &tmp_ray.collision_count,
					(t_collision){intersection, &global->lines[i], face});
			if (!tmp)
				return ;
			tmp_ray.collisions = tmp;
		}
	}
	for (int i = 0; i < tmp_ray.collision_count; i++)
	{
		distance = get_distance(global->player->pos,
				tmp_ray.collisions[i].point);
		if (distance < min_distance)
		{
			min_distance = distance;
			tmp_ray.closest_collision = &tmp_ray.collisions[i];
		}
	}
	for (int i = 0; i < global->door_count; i++)
	{
		distance = point_line_distance(global->player->pos,
				global->door_line[i]) / global->scale_factor;
		if (distance < INTERACT_DISTANCE && distance > 1.0
			&& (tmp_ray.closest_collision->line->type == DOOR
				|| tmp_ray.closest_collision->line->type == DOOR_SIDE))
			global->door_line[i]->door->state = CLOSING;
	}
}

void	check_active_lines(t_global *global)
{
	float		distance;
	t_ray		tmp_ray;
	t_collision	*tmp;
	t_vec2d		intersection;
	t_face		face;
	float		min_distance;

	min_distance = 1000000;
	tmp_ray.origin = global->player->pos;
	tmp_ray.direction = global->player->dir;
	tmp_ray.collisions = NULL;
	tmp_ray.collision_count = 0;
	for (int i = 0; i < global->line_count; i++)
	{
		intersection = ray_line_collision(&tmp_ray, &global->lines[i], &face);
		if (intersection.x != -1)
		{
			tmp = new_collision(tmp_ray.collisions, &tmp_ray.collision_count,
					(t_collision){intersection, &global->lines[i], face});
			if (!tmp)
				return ;
			tmp_ray.collisions = tmp;
		}
	}
	for (int i = 0; i < tmp_ray.collision_count; i++)
	{
		distance = get_distance(global->player->pos,
				tmp_ray.collisions[i].point);
		if (distance < min_distance)
		{
			min_distance = distance;
			tmp_ray.closest_collision = &tmp_ray.collisions[i];
		}
	}
	for (int i = 0; i < global->door_count; i++)
	{
		distance = point_line_distance(global->player->pos,
				global->door_line[i]) / global->scale_factor;
		if (distance < INTERACT_DISTANCE && distance > 1.0
			&& (tmp_ray.closest_collision->line->type == DOOR
				|| tmp_ray.closest_collision->line->type == DOOR_SIDE))
			global->door_line[i]->door->state = OPENING;
	}
}

void	update_door_segments(t_global *global)
{
	float	scaled_x;
	float	scaled_y;
	float	distance;
	float	speed;

	speed = (global->scale_factor / 40);
	for (int i = 0; i < global->door_count; i++)
	{
		distance = point_line_distance(global->player->pos,
				global->door_line[i]);
		if (global->doors[i].state == OPENING && distance > 1.0
			/ global->scale_factor)
		{
			if (global->door_line[i]->alignment == VERTICAL)
			{
				scaled_y = global->door_line[i]->a.y / global->scale_factor;
				if (scaled_y > global->door_line[i]->open_end.y - 2)
				{
					global->door_line[i]->a.y -= speed;
					global->door_line[i]->b.y -= speed;
				}
				if (scaled_y <= global->door_line[i]->open_end.y - 2)
					global->doors[i].state = OPEN;
			}
			else
			{
				scaled_x = global->door_line[i]->a.x / global->scale_factor;
				if (scaled_x < global->door_line[i]->open_end.x)
				{
					global->door_line[i]->a.x += speed;
					global->door_line[i]->b.x += speed;
				}
				if (scaled_x >= global->door_line[i]->open_end.x)
					global->doors[i].state = OPEN;
			}
		}
		else if (global->doors[i].state == CLOSING && distance > 1.0
			/ global->scale_factor)
		{
			if (global->door_line[i]->alignment == VERTICAL)
			{
				scaled_y = global->door_line[i]->a.y / global->scale_factor;
				if (scaled_y < global->door_line[i]->close_end.y)
				{
					global->door_line[i]->a.y += speed;
					global->door_line[i]->b.y += speed;
				}
				if (scaled_y >= global->door_line[i]->close_end.y)
					global->doors[i].state = CLOSED;
			}
			else
			{
				scaled_x = global->door_line[i]->a.x / global->scale_factor;
				if (scaled_x > global->door_line[i]->close_end.x)
				{
					global->door_line[i]->a.x -= speed;
					global->door_line[i]->b.x -= speed;
				}
				if (scaled_x <= global->door_line[i]->close_end.x)
					global->doors[i].state = CLOSED;
			}
		}
	}
}
