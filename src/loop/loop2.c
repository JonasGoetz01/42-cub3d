/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:03:03 by jgotz             #+#    #+#             */
/*   Updated: 2024/07/22 11:36:53 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

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
