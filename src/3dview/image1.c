/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:26:41 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 12:12:01 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

mlx_texture_t	*load_texture(t_global *global, t_collision *closest_collision)
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
	if (closest_collision->face == NORTH)
		return (texture_north);
	else if (closest_collision->face == SOUTH)
		return (texture_south);
	else if (closest_collision->face == EAST)
		return (texture_east);
	else if (closest_collision->face == WEST)
		return (texture_west);
	else
		return (door);
}

float	calculate_hit_percentage(t_collision *closest_collision)
{
	if (closest_collision->line->alignment == VERTICAL)
	{
		return ((closest_collision->point.y - closest_collision->line->a.y)
			/ (closest_collision->line->b.y - closest_collision->line->a.y));
	}
	else
	{
		return ((closest_collision->point.x - closest_collision->line->a.x)
			/ (closest_collision->line->b.x - closest_collision->line->a.x));
	}
}

void	draw_texture_column(t_global *global, int bar_height, int top_y, int x,
		mlx_texture_t *texture, int texture_x)
{
	int		texture_y;
	int		color;
	uint8_t	*pixel;
	int		j;

	j = 0;
	while (j < bar_height)
	{
		texture_y = (int)(((float)j / bar_height) * (texture->height));
		texture_y = fmax(fmin(texture_y, texture->height - 1), 0);
		texture_x = fmax(fmin(texture_x, texture->width - 1), 0);
		pixel = &(texture->pixels[(texture_y * texture->width + texture_x)
				* texture->bytes_per_pixel]);
		if (pixel != NULL)
		{
			color = get_rgba(pixel[0], pixel[1], pixel[2], 255);
			if (top_y + j >= 0 && (uint32_t)top_y + j < global->img->height)
			{
				if (x >= 0 && (uint32_t)x < global->img->width)
				{
					mlx_put_pixel(global->img, x, top_y + j, color);
				}
			}
		}
		j++;
	}
}

void	process_ray(t_global *global, int i, float player_angle,
		float *z_buffer)
{
	t_ray			*ray;
	t_collision		*closest_collision;
	int				bar_height;
	float			hit_percentage;
	mlx_texture_t	*texture;

	ray = &global->player->rays[i];
	closest_collision = ray->closest_collision;
	if (closest_collision)
	{
		bar_height = map_distance_to_height(fmaxf(get_distance(global->player->pos,
						closest_collision->point), 0.1f)
				* cosf(atan2f(ray->direction.y, ray->direction.x)
					- player_angle), global);
		texture = load_texture(global, closest_collision);
		hit_percentage = calculate_hit_percentage(closest_collision);
		hit_percentage = fmax(fmin(hit_percentage, 1.0f), 0.0f);
		draw_texture_column(global, bar_height, global->img->height / 2
			- (bar_height / 2), i, texture, (int)(hit_percentage
				* (texture->width)));
		z_buffer[i] = (float)fmaxf(get_distance(global->player->pos,
					closest_collision->point), 0.1f)
			* cosf(atan2f(ray->direction.y, ray->direction.x) - player_angle);
	}
}

void	render_3d(t_global *global)
{
	int		i;
	float	player_angle;
	float	z_buffer[WIDTH];

	player_angle = atan2(global->player->dir.y, global->player->dir.x);
	if (global->close)
		check_inactive_lines(global);
	if (global->open)
		check_active_lines(global);
	i = 0;
	while (i < (int)global->img->width)
	{
		process_ray(global, i, player_angle, z_buffer);
		i++;
	}
}
