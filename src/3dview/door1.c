/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:26:41 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 13:38:42 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	detect_collisions(t_global *global, t_ray *tmp_ray)
{
	t_vec2d		intersection;
	t_collision	*tmp;
	t_face		face;
	int			i;

	tmp_ray->collisions = NULL;
	tmp_ray->collision_count = 0;
	i = -1;
	while (++i < global->line_count)
	{
		intersection = ray_line_collision(tmp_ray, &global->lines[i], &face);
		if (intersection.x != -1)
		{
			tmp = new_collision(tmp_ray->collisions, &tmp_ray->collision_count,
					(t_collision){intersection, &global->lines[i], face});
			if (!tmp)
				return ;
			tmp_ray->collisions = tmp;
		}
	}
}

void	find_closest_collision_door(t_ray *tmp_ray)
{
	float	distance;
	float	min_distance;
	int		i;

	min_distance = 1000000;
	i = -1;
	while (++i < tmp_ray->collision_count)
	{
		distance = get_distance(tmp_ray->origin, tmp_ray->collisions[i].point);
		if (distance < min_distance)
		{
			min_distance = distance;
			tmp_ray->closest_collision = &tmp_ray->collisions[i];
		}
	}
}
