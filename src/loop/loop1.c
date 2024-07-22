/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:03:03 by jgotz             #+#    #+#             */
/*   Updated: 2024/07/22 11:36:32 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	put_pixel_if_visible(t_global *global, int x, int y, int color)
{
	if ((uint32_t)x < global->img->height && (uint32_t)y < global->img->width)
	{
		mlx_put_pixel(global->img, y, x, color);
	}
}

void	process_pixel(t_global *global, mlx_texture_t *texture, t_vec2d iter,
		t_vec2d start)
{
	uint8_t	*pixel;
	int		color;

	pixel = &(texture->pixels[(((int)iter.x - (int)start.x)
				* (int)texture->width + ((int)iter.y - (int)start.y))
			* (int)texture->bytes_per_pixel]);
	if (pixel != NULL)
	{
		color = get_rgba(pixel[0], pixel[1], pixel[2], 255);
		if (pixel[3] != 0)
		{
			put_pixel_if_visible(global, iter.x, iter.y, color);
		}
	}
}

void	ft_texture_to_image(t_global *global, mlx_texture_t *texture)
{
	t_vec2d	iter;
	t_vec2d	start;

	start.x = (int)global->window_height - (int)texture->height;
	iter.x = start.x;
	while (iter.x < global->window_height)
	{
		start.y = (int)global->window_width - (int)texture->width;
		iter.y = start.y;
		while (iter.y < global->window_width)
		{
			process_pixel(global, texture, iter, start);
			iter.y++;
		}
		iter.x++;
	}
}

// @TODO test if it works well on 42 mac
void	my_usleep(unsigned int microseconds)
{
	unsigned long	wait_time;
	unsigned long	start_time;
	unsigned long	current_time;

	wait_time = microseconds / 1000;
	start_time = get_current_millis();
	current_time = get_current_millis();
	while ((current_time - start_time) < wait_time)
		current_time = get_current_millis();
}

void	cap_fps(double *elapsed_time, double frame_duration, t_global *global)
{
	double	current_time;

	if (*elapsed_time < frame_duration)
	{
		my_usleep((frame_duration - *elapsed_time) * 1000);
		current_time = get_current_millis();
		*elapsed_time = current_time - global->time;
		global->time = current_time;
	}
}
