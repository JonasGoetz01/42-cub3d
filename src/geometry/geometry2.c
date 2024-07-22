/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:03:03 by jgotz             #+#    #+#             */
/*   Updated: 2024/07/22 10:09:58 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

t_vec2d	calculate_collision_point(float t, t_vec2d ray_origin,
		t_vec2d ray_direction)
{
	return ((t_vec2d){ray_origin.x + t * (ray_direction.x), ray_origin.y + t
		* (ray_direction.y)});
}

void	determine_face(t_vec2d ray_origin, t_vec2d collision_point,
		t_line *line, t_face *face)
{
	if (line->alignment == HORIZONTAL)
	{
		if (line->type == WALL)
		{
			if (ray_origin.y < collision_point.y)
				*face = SOUTH;
			else
				*face = NORTH;
		}
		else
			*face = DOORS;
	}
	else
	{
		if (line->type == WALL)
		{
			if (ray_origin.x < collision_point.x)
				*face = EAST;
			else
				*face = WEST;
		}
		else
			*face = DOORS;
	}
}

bool	is_collision(float t, float u)
{
	return (t >= 0 && u >= 0 && u <= 1);
}

t_vec2d	ray_line_intersection(t_ray *ray, t_line *line, float *t, float *u)
{
	float	x2;
	float	y2;
	float	denominator;

	x2 = ray->origin.x + ray->direction.x;
	y2 = ray->origin.y + ray->direction.y;
	denominator = (ray->origin.x - x2) * (line->a.y - line->b.y)
		- (ray->origin.y - y2) * (line->a.x - line->b.x);
	if (denominator == 0)
		return ((t_vec2d){-1, -1});
	*t = ((ray->origin.x - line->a.x) * (line->a.y - line->b.y) - (ray->origin.y
				- line->a.y) * (line->a.x - line->b.x)) / denominator;
	*u = ((ray->origin.x - line->a.x) * (ray->origin.y - y2) - (ray->origin.y
				- line->a.y) * (ray->origin.x - x2)) / denominator;
	return ((t_vec2d){0, 0});
}

t_vec2d	ray_line_collision(t_ray *ray, t_line *line, t_face *face)
{
	t_vec2d	collision_point;
	float	t;
	float	u;

	collision_point = ray_line_intersection(ray, line, &t, &u);
	if (collision_point.x == -1 && collision_point.y == -1)
		return (collision_point);
	if (is_collision(t, u))
	{
		collision_point = calculate_collision_point(t, ray->origin,
				ray->direction);
		determine_face(ray->origin, collision_point, line, face);
		return (collision_point);
	}
	return ((t_vec2d){-1, -1});
}
