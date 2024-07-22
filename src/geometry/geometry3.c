/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:03:03 by jgotz             #+#    #+#             */
/*   Updated: 2024/07/22 11:06:22 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

t_collision	*new_collision(t_collision *collisions, int *collision_count,
		t_collision n_collision)
{
	t_collision	*new_collisions;

	new_collisions = ft_realloc(collisions, *collision_count
			* sizeof(t_collision), (*collision_count + 1)
			* sizeof(t_collision));
	if (!new_collisions)
		return (NULL);
	new_collisions[*collision_count].point = n_collision.point;
	new_collisions[*collision_count].line = n_collision.line;
	new_collisions[*collision_count].face = n_collision.face;
	(*collision_count)++;
	return (new_collisions);
}

t_collision	*process_intersections(t_ray *ray, t_line *lines, int line_count)
{
	t_vec2d		intersection;
	t_face		face;
	t_collision	*new_collisions;
	int			j;

	j = 0;
	while (j < line_count)
	{
		intersection = ray_line_collision(ray, &lines[j], &face);
		if (intersection.x != -1)
		{
			new_collisions = new_collision(ray->collisions,
					&ray->collision_count, (t_collision){intersection,
					&lines[j], face});
			if (!new_collisions)
				return (NULL);
			ray->collisions = new_collisions;
		}
		j++;
	}
	return (ray->collisions);
}

t_collision	*find_closest_collision(t_ray *ray, t_vec2d player_pos)
{
	float		min_distance;
	float		distance;
	t_collision	*closest_collision;
	int			j;

	min_distance = 1000000;
	closest_collision = NULL;
	j = 0;
	while (j < ray->collision_count)
	{
		distance = sqrtf(powf(player_pos.x - ray->collisions[j].point.x, 2)
				+ powf(player_pos.y - ray->collisions[j].point.y, 2));
		if (distance < min_distance)
		{
			min_distance = distance;
			closest_collision = &ray->collisions[j];
		}
		j++;
	}
	return (closest_collision);
}

void	raycast(t_global *global)
{
	t_collision	*new_collisions;
	t_collision	*closest_collision;
	t_ray		*ray;
	int			i;

	i = 0;
	while (i < (int)global->img->width)
	{
		ray = &global->player->rays[i];
		new_collisions = process_intersections(ray, global->lines,
				global->line_count);
		if (!new_collisions)
			return ;
		closest_collision = find_closest_collision(ray, global->player->pos);
		ray->closest_collision = closest_collision;
		i++;
	}
}

double	point_line_distance(t_vec2d point, t_line *line)
{
	double	dx;
	double	dy;
	double	t;
	double	closest_x;
	double	closest_y;

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
	closest_x = line->a.x + t * dx;
	closest_y = line->a.y + t * dy;
	dx = point.x - closest_x;
	dy = point.y - closest_y;
	return (sqrt(dx * dx + dy * dy));
}
