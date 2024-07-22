/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:26:41 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 13:55:45 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	collision_helper(t_ray *ray, t_player *player, t_vec2d dir,
		float ray_angle)
{
	ray->origin = (t_vec2d){player->pos.x - dir.x, player->pos.y - dir.y};
	ray->direction = (t_vec2d){cosf(ray_angle), sinf(ray_angle)};
	ray->collisions = NULL;
	ray->collision_count = 0;
}

t_player	*new_player(t_global *global, t_vec2d pos, t_vec2d dir)
{
	t_player	*player;
	float		angle_increment;
	float		ray_angle;
	int			i;

	player = malloc(sizeof(t_player));
	if (!player)
		ft_exit_free(global);
	player->rays = NULL;
	player->pos = pos;
	player->dir = dir;
	player->rays = malloc(sizeof(t_ray) * global->img->width);
	if (!player->rays)
		ft_exit_free(global);
	angle_increment = FOV / (global->img->width - 1);
	i = 0;
	while (i < (int)global->img->width)
	{
		ray_angle = atan2f(player->dir.y, player->dir.x) - (float)(FOV / 2.0f)
			+ ((float)i * angle_increment);
		collision_helper(&(player->rays[i]), player, dir, ray_angle);
		i++;
	}
	return (player);
}

t_vec2d	calculate_perpendicular_direction(t_line dir_line, t_line wall)
{
	t_vec2d	wall_dir;
	t_vec2d	perpendicular;
	t_vec2d	dir_vector;
	t_vec2d	new_dir;
	float	length;

	wall_dir.x = wall.b.x - wall.a.x;
	wall_dir.y = wall.b.y - wall.a.y;
	perpendicular.x = -wall_dir.y;
	perpendicular.y = wall_dir.x;
	length = sqrtf(perpendicular.x * perpendicular.x + perpendicular.y
			* perpendicular.y);
	perpendicular.x /= length;
	perpendicular.y /= length;
	dir_vector.x = dir_line.b.x - dir_line.a.x;
	dir_vector.y = dir_line.b.y - dir_line.a.y;
	new_dir.x = dir_vector.x * perpendicular.x + dir_vector.y * perpendicular.y
		* perpendicular.x;
	new_dir.y = dir_vector.x * perpendicular.x + dir_vector.y * perpendicular.y
		* perpendicular.y;
	return (new_dir);
}

bool	line_line_collision(t_line *a, t_line *b)
{
	float	denominator;
	float	ua;
	float	ub;

	denominator = ((b->b.y - b->a.y) * (a->b.x - a->a.x) - (b->b.x - b->a.x)
			* (a->b.y - a->a.y));
	if (denominator == 0)
		return (false);
	ua = ((b->b.x - b->a.x) * (a->a.y - b->a.y) - (b->b.y - b->a.y) * (a->a.x
				- b->a.x)) / denominator;
	ub = ((a->b.x - a->a.x) * (a->a.y - b->a.y) - (a->b.y - a->a.y) * (a->a.x
				- b->a.x)) / denominator;
	return (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1);
}

bool	circle_line_collision(t_vec2d circle_center, float radius, t_line line)
{
	t_vec2d	line_vec;
	t_vec2d	closest_point;
	float	line_length;
	float	t;
	float	distance_squared;

	line_vec.x = line.b.x - line.a.x;
	line_vec.y = line.b.y - line.a.y;
	line_length = sqrtf(line_vec.x * line_vec.x + line_vec.y * line_vec.y);
	if (line_length == 0)
		return (false);
	t = ((circle_center.x - line.a.x) * line_vec.x + (circle_center.y
				- line.a.y) * line_vec.y) / (line_length * line_length);
	t = fmaxf(0, fminf(1, t));
	closest_point.x = line.a.x + t * line_vec.x;
	closest_point.y = line.a.y + t * line_vec.y;
	distance_squared = powf(circle_center.x - closest_point.x, (float)2)
		+ powf(circle_center.y - closest_point.y, (float)2);
	return (distance_squared <= (radius * radius));
}
