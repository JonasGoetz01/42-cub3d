/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:26:41 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 13:09:16 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	load_texture_png(t_global *global, mlx_texture_t **texture_north,
		mlx_texture_t **texture_south, mlx_texture_t **texture_east)
{
	if (!*texture_north)
		*texture_north = mlx_load_png(global->texture->north);
	if (!*texture_south)
		*texture_south = mlx_load_png(global->texture->south);
	if (!*texture_east)
		*texture_east = mlx_load_png(global->texture->east);
}

mlx_texture_t	*load_texture(t_global *global, t_collision *closest_collision)
{
	static mlx_texture_t	*texture_north = NULL;
	static mlx_texture_t	*texture_south = NULL;
	static mlx_texture_t	*texture_east = NULL;
	static mlx_texture_t	*texture_west = NULL;
	static mlx_texture_t	*door = NULL;

	load_texture_png(global, &texture_north, &texture_south, &texture_east);
	if (!texture_west)
		texture_west = mlx_load_png(global->texture->west);
	if (!door)
		door = mlx_load_png("textures/diamond.png");
	if (!texture_north || !texture_south || !texture_east || !texture_west
		|| !door)
		ft_exit_free(global);
	if (closest_collision->face == NORTH)
		return (texture_north);
	if (closest_collision->face == SOUTH)
		return (texture_south);
	if (closest_collision->face == EAST)
		return (texture_east);
	if (closest_collision->face == WEST)
		return (texture_west);
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

void	draw_texture_collumn_helper(t_vec2d position, t_global *global,
		int color, int j)
{
	if ((int)position.x >= 0 && (uint32_t)position.x < global->img->width)
	{
		mlx_put_pixel(global->img, (int)position.x, position.y + j, color);
	}
}

void	draw_texture_column(t_global *global, t_vec2d position,
		mlx_texture_t *texture, t_vec2d texture_coords)
{
	int		texture_y;
	int		color;
	uint8_t	*pixel;
	int		j;

	j = 0;
	while (j < texture_coords.y)
	{
		texture_y = (int)(((float)j / texture_coords.y) * (texture->height));
		texture_y = fmax(fmin(texture_y, texture->height - 1), 0);
		texture_coords.x = fmax(fmin(texture_coords.x, texture->width - 1), 0);
		pixel = &(texture->pixels[(texture_y * texture->width
					+ (int)texture_coords.x) * texture->bytes_per_pixel]);
		if (pixel)
		{
			color = get_rgba(pixel[0], pixel[1], pixel[2], 255);
			if (position.y + j >= 0 && (uint32_t)(position.y
				+ j) < global->img->height)
				draw_texture_collumn_helper(position, global, color, j);
		}
		j++;
	}
}
