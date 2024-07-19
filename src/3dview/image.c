/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 15:48:32 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/19 15:48:35 by cgerling         ###   ########.fr       */
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

void	check_close_door(t_global *global)
{
	float		distance;
	t_ray		tmp_ray;
	int			i;

	tmp_ray.origin = global->player->pos;
	tmp_ray.direction = global->player->dir;
	detect_collisions(global, &tmp_ray);
	find_closest_collision(&tmp_ray);
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
	float		distance;
	t_ray		tmp_ray;
	int			i;

	tmp_ray.origin = global->player->pos;
	tmp_ray.direction = global->player->dir;
	detect_collisions(global, &tmp_ray);
	find_closest_collision(&tmp_ray);
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
		if (scaled_y > global->door_line[i]->open_end.y - 2)
		{
			global->door_line[i]->a.y -= speed;
			global->door_line[i]->b.y -= speed;
		}
		if (scaled_y <= global->door_line[i]->open_end.y - 2)
			global->door_line[i]->door->state = OPEN;
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
				global->door_line[i]);
		if (distance > INTERACT_MIN / global->scale_factor)
		{
			if (global->door_line[i]->door->state == OPENING)
				update_door_opening(global, i, speed);
			else if (global->door_line[i]->door->state == CLOSING)
				update_door_closing(global, i, speed);
		}
	}
}

void	render_3d(t_global *global)
{
	int bar_width;
	int i;
	t_ray *ray;
	t_collision *closest_collision;
	float distance;
	int bar_height;
	int center_y;
	int top_y;
	int bottom_y;
	int x;
	float hit_percentage;
	int color;
	mlx_texture_t *texture;
	uint8_t *pixel;
	int draw_y;
	int texture_x;
	int texture_y;
	float z_buffer[global->img->width];
	float perpendicular_distance;
	float ray_angle;
	float player_angle;
	float angle_diff;
	static mlx_texture_t *texture_north = NULL;
	static mlx_texture_t *texture_south = NULL;
	static mlx_texture_t *texture_east = NULL;
	static mlx_texture_t *texture_west = NULL;
	static mlx_texture_t *door = NULL;
	int r;
	int g;
	int b;
	int j;

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
	player_angle = atan2(global->player->dir.y, global->player->dir.x);
	bar_width = 1;
	if (global->close)
		check_close_door(global);
	if (global->open)
		check_open_door(global);
	for (i = 0; i < (int)global->img->width; i++)
	{
		ray = &global->player->rays[i];
		closest_collision = ray->closest_collision;
		if (closest_collision)
		{
			distance = get_distance(global->player->pos,
					closest_collision->point);
			distance = fmax(distance, 0.1f);
			ray_angle = atan2(ray->direction.y, ray->direction.x);
			angle_diff = ray_angle - player_angle;
			perpendicular_distance = distance * cos(angle_diff);
			bar_height = map_distance_to_height(perpendicular_distance, global);
			center_y = global->img->height / 2;
			top_y = center_y - (bar_height / 2);
			bottom_y = center_y + (bar_height / 2);
			x = i * bar_width;
			if (closest_collision->face == NORTH)
				texture = texture_north;
			else if (closest_collision->face == SOUTH)
				texture = texture_south;
			else if (closest_collision->face == EAST)
				texture = texture_east;
			else if (closest_collision->face == WEST)
				texture = texture_west;
			else if (closest_collision->face == DOORS)
				texture = door;
			else
				texture = texture_north;
			if (closest_collision->line->alignment == VERTICAL)
			{
				hit_percentage = (closest_collision->point.y
						- closest_collision->line->a.y)
					/ (closest_collision->line->b.y
						- closest_collision->line->a.y);
			}
			else
			{
				hit_percentage = (closest_collision->point.x
						- closest_collision->line->a.x)
					/ (closest_collision->line->b.x
						- closest_collision->line->a.x);
			}
			hit_percentage = fmax(fmin(hit_percentage, 1.0f), 0.0f);
			texture_x = (int)(hit_percentage * (texture->width));
			j = 0;
			while (j < bar_height)
			{
				texture_y = (int)(((float)j / bar_height) * (texture->height));
				if ((uint32_t)texture_y >= texture->height)
					texture_y = texture->height - 1;
				if (texture_y < 0)
					texture_y = 0;
				if ((uint32_t)texture_x >= texture->width)
					texture_x = texture->width - 1;
				if (texture_x < 0)
					texture_x = 0;
				pixel = &(texture->pixels[(texture_y * texture->width
							+ texture_x) * texture->bytes_per_pixel]);
				if (pixel != NULL)
				{
					r = pixel[0];
					g = pixel[1];
					b = pixel[2];
					color = get_rgba(r, g, b, 255);
					draw_y = top_y + j;
					if (draw_y >= 0 && (uint32_t)draw_y < global->img->height)
					{
						if (x >= 0 && (uint32_t)x < global->img->width)
						{
							mlx_put_pixel(global->img, x, draw_y, color);
						}
					}
				}
				j++;
			}
			z_buffer[i] = perpendicular_distance;
		}
	}
}