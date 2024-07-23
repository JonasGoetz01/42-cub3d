/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:26:41 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 19:07:02 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	check_close_door(t_global *global)
{
	float	distance;
	t_ray	tmp_ray;
	int		i;

	tmp_ray.origin = global->player->pos;
	tmp_ray.direction = global->player->dir;
	detect_collisions(global, &tmp_ray);
	find_closest_collision_door(&tmp_ray);
	i = -1;
	while (++i < global->door_count)
	{
		distance = point_line_distance(global->player->pos,
				global->door_line[i]) / global->scale_factor;
		if (distance < INTERACT_MAX && distance > INTERACT_MIN
			&& (tmp_ray.closest_collision->line->type == DOOR
				|| tmp_ray.closest_collision->line->type == DOOR_SIDE))
			global->door_line[i]->door->state = CLOSING;
	}
	if (tmp_ray.collisions)
		free(tmp_ray.collisions);
}

void	check_open_door(t_global *global)
{
	float	distance;
	t_ray	tmp_ray;
	int		i;

	tmp_ray.origin = global->player->pos;
	tmp_ray.direction = global->player->dir;
	detect_collisions(global, &tmp_ray);
	find_closest_collision_door(&tmp_ray);
	i = -1;
	while (++i < global->door_count)
	{
		distance = point_line_distance(global->player->pos,
				global->door_line[i]) / global->scale_factor;
		if (distance < INTERACT_MAX && distance > INTERACT_MIN
			&& (tmp_ray.closest_collision->line->type == DOOR
				|| tmp_ray.closest_collision->line->type == DOOR_SIDE))
			global->door_line[i]->door->state = OPENING;
	}
	if (tmp_ray.collisions)
		free(tmp_ray.collisions);
}

void	update_door_opening(t_global *global, int i, float speed)
{
	float	scaled_x;
	float	scaled_y;

	if (global->door_line[i]->alignment == VERTICAL)
	{
		scaled_y = global->door_line[i]->a.y / global->scale_factor;
		if (scaled_y > global->door_line[i]->open_end.y - 1.99)
		{
			global->door_line[i]->a.y -= speed;
			global->door_line[i]->b.y -= speed;
		}
		if (scaled_y <= global->door_line[i]->open_end.y - 1.99)
			global->door_line[i]->door->state = OPEN;
	}
	else
	{
		scaled_x = global->door_line[i]->a.x / global->scale_factor;
		if (scaled_x < global->door_line[i]->open_end.x - 0.01)
		{
			global->door_line[i]->a.x += speed;
			global->door_line[i]->b.x += speed;
		}
		if (scaled_x >= global->door_line[i]->open_end.x - 0.01)
			global->door_line[i]->door->state = OPEN;
	}
}

void	update_door_closing(t_global *global, int i, float speed)
{
	float	scaled_x;
	float	scaled_y;

	if (global->door_line[i]->alignment == VERTICAL)
	{
		scaled_y = global->door_line[i]->a.y / global->scale_factor;
		if (scaled_y < global->door_line[i]->close_end.y)
		{
			global->door_line[i]->a.y += speed;
			global->door_line[i]->b.y += speed;
		}
		if (scaled_y >= global->door_line[i]->close_end.y)
			global->door_line[i]->door->state = CLOSED;
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
			global->door_line[i]->door->state = CLOSED;
	}
}

void	update_door_lines(t_global *global)
{
	float	distance;
	float	speed;
	int		i;

	speed = (global->scale_factor / 40);
	i = -1;
	while (++i < global->door_count)
	{
		distance = point_line_distance(global->player->pos,
				global->door_line[i]) / global->scale_factor;
		if (distance > INTERACT_MIN)
		{
			if (global->door_line[i]->door->state == OPENING)
				update_door_opening(global, i, speed);
			else if (global->door_line[i]->door->state == CLOSING)
				update_door_closing(global, i, speed);
		}
	}
}
