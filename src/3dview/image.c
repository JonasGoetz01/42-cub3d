/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 15:48:32 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 11:13:23 by cgerling         ###   ########.fr       */
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

void	show_sky_and_floor(t_global *global)
{
	int	x;
	int	y;

	y = -1;
	while (++y < global->window_height / 2)
	{
		x = 0;
		while (x < global->window_width)
		{
			mlx_put_pixel(global->img, x, y, get_rgba(global->ceiling.r,
					global->ceiling.g, global->ceiling.b, 255));
			x++;
		}
	}
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
					intersection, &global->lines[i], face);
			if (!tmp)
				return ;
			tmp_ray->collisions = tmp;
		}
	}
}

void	find_closest_collision(t_ray *tmp_ray)
{
	float		distance;
	float		min_distance;
	int			i;

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

void	render_3d(t_global *global)
{
	static mlx_texture_t	*texture_north = NULL;
	static mlx_texture_t	*texture_south = NULL;
	static mlx_texture_t	*texture_east = NULL;
	static mlx_texture_t	*texture_west = NULL;
	static mlx_texture_t	*door = NULL;

	if (!texture_north)
		texture_north = mlx_load_png(global->texture->north);
	if (!texture_south)
		texture_south = mlx_load_png(global->texture->south);
	if (!texture_east)
		texture_east = mlx_load_png(global->texture->east);
	if (!texture_west)
		texture_west = mlx_load_png(global->texture->west);
	if (!door)
		door = mlx_load_png("textures/diamond.png");
	if (!texture_north || !texture_south || !texture_east || !texture_west
		|| !door)
		ft_exit_free(global);
	global->ren.player_angle = atan2(global->player->dir.y,
			global->player->dir.x);
	global->ren.bar_width = 1;
	if (global->close)
		check_close_door(global);
	if (global->open)
		check_open_door(global);
	global->ren.i = 0;
	while (global->ren.i < (int)global->img->width)
	{
		global->ren.ray = &global->player->rays[global->ren.i];
		global->ren.closest_collision = global->ren.ray->closest_collision;
		if (global->ren.closest_collision)
		{
			global->ren.distance = get_distance(global->player->pos,
					global->ren.closest_collision->point);
			global->ren.distance = fmax(global->ren.distance, 0.1f);
			global->ren.ray_angle = atan2(global->ren.ray->direction.y,
					global->ren.ray->direction.x);
			global->ren.angle_diff = global->ren.ray_angle
				- global->ren.player_angle;
			global->ren.perpendicular_distance = global->ren.distance
				* cos(global->ren.angle_diff);
			global->ren.bar_height = map_distance_to_height(global->ren.perpendicular_distance, global);
			global->ren.center_y = global->img->height / 2;
			global->ren.top_y = global->ren.center_y
				- (global->ren.bar_height / 2);
			global->ren.bottom_y = global->ren.center_y
				+ (global->ren.bar_height / 2);
			global->ren.x = global->ren.i * global->ren.bar_width;
			if (global->ren.closest_collision->face == NORTH)
				global->ren.texture = *texture_north;
			else if (global->ren.closest_collision->face == SOUTH)
				global->ren.texture = *texture_south;
			else if (global->ren.closest_collision->face == EAST)
				global->ren.texture = *texture_east;
			else if (global->ren.closest_collision->face == WEST)
				global->ren.texture = *texture_west;
			else if (global->ren.closest_collision->face == DOORS)
				global->ren.texture = *door;
			else
				global->ren.texture = *texture_north;
			if (global->ren.closest_collision->line->alignment == VERTICAL)
			{
				global->ren.hit_percentage = (global->ren.closest_collision->point.y
						- global->ren.closest_collision->line->a.y)
					/ (global->ren.closest_collision->line->b.y
						- global->ren.closest_collision->line->a.y);
			}
			else
			{
				global->ren.hit_percentage = (global->ren.closest_collision->point.x
						- global->ren.closest_collision->line->a.x)
					/ (global->ren.closest_collision->line->b.x
						- global->ren.closest_collision->line->a.x);
			}
			global->ren.hit_percentage = fmax(fmin(global->ren.hit_percentage, 1.0f), 0.0f);
			global->ren.texture_x = (int)(global->ren.hit_percentage
				* (global->ren.texture.width));
			global->ren.j = 0;
			while (global->ren.j < global->ren.bar_height)
			{
				global->ren.texture_y = (int)(((float)global->ren.j
							/ global->ren.bar_height)
						* (global->ren.texture.height));
				if ((uint32_t)global->ren.texture_y >= global->ren.texture.height)
					global->ren.texture_y = global->ren.texture.height - 1;
				if (global->ren.texture_y < 0)
					global->ren.texture_y = 0;
				if ((uint32_t)global->ren.texture_x >= global->ren.texture.width)
					global->ren.texture_x = global->ren.texture.width - 1;
				if (global->ren.texture_x < 0)
					global->ren.texture_x = 0;
				global->ren.pixel = &(global->ren.texture.pixels[(global->ren.texture_y * global->ren.texture.width
							+ global->ren.texture_x) * global->ren.texture.bytes_per_pixel]);
				if (global->ren.pixel != NULL)
				{
					global->ren.r = global->ren.pixel[0];
					global->ren.g = global->ren.pixel[1];
					global->ren.b = global->ren.pixel[2];
					global->ren.color = get_rgba(global->ren.r,
						global->ren.g, global->ren.b, 255);
					global->ren.draw_y = global->ren.top_y
						+ global->ren.j;
					if (global->ren.draw_y >= 0 && (uint32_t)global->ren.draw_y < global->img->height)
					{
						if (global->ren.x >= 0 && (uint32_t)global->ren.x < global->img->width)
							mlx_put_pixel(global->img, global->ren.x, global->ren.draw_y, global->ren.color);
					}
				}
				global->ren.j++;
			}
			global->ren.z_buffer[global->ren.i]
				= global->ren.perpendicular_distance;
		}
		global->ren.i++;
	}
}
