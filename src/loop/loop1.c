/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:03:03 by jgotz             #+#    #+#             */
/*   Updated: 2024/07/22 11:25:54 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	ft_texture_to_image(t_global *global, mlx_texture_t *texture)
{
	int		i;
	int		i_start;
	int		j;
	int		j_start;
	int		color;
	uint8_t	*pixel;

	i = (int)global->window_height - (int)texture->height;
	i_start = i;
	while (i < global->window_height)
	{
		j = (int)global->window_width - (int)texture->width;
		j_start = j;
		while (j < global->window_width)
		{
			pixel = &(texture->pixels[((i - i_start) * texture->width + (j
							- j_start)) * texture->bytes_per_pixel]);
			if (pixel != NULL)
			{
				color = get_rgba(pixel[0], pixel[1], pixel[2], 255);
				if (pixel[3] != 0)
				{
					if ((uint32_t)i < global->img->height
						&& (uint32_t)j < global->img->width)
					{
						mlx_put_pixel(global->img, j, i, color);
					}
				}
			}
			j++;
		}
		i++;
	}
}

// @TODO test if it works well on 42 mac
void	my_usleep(unsigned int microseconds)
{
	unsigned long	wait_time;

	unsigned long start_time, current_time;
	wait_time = microseconds / 1000;
	start_time = get_current_millis();
	do
	{
		current_time = get_current_millis();
	} while ((current_time - start_time) < wait_time);
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

void	display_fps(int *fps_timer, double elapsed_time)
{
	int	fps;

	if (*fps_timer % 10 == 0)
	{
		fps = (int)(1000.0 / elapsed_time);
		write(1, "FPS: ", 5);
		ft_putnbr_fd(fps, 1);
		write(1, "\r", 1);
		*fps_timer = 0;
	}
	(*fps_timer)++;
}

void	update_images(t_global *global)
{
	mlx_delete_image(global->mlx, global->minimap);
	mlx_delete_image(global->mlx, global->img);
	global->minimap = mlx_new_image(global->mlx, global->window_width
			* global->minimap_scale, global->window_height
			* global->minimap_scale);
	global->img = mlx_new_image(global->mlx, global->window_width,
			global->window_height);
}
