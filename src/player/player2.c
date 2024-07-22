/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:26:41 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 13:55:40 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	rotate_player(t_global *global, float angle)
{
	t_vec2d	old_dir;
	t_vec2d	new_dir;
	float	angle_increment;
	float	new_angle;
	int		i;

	old_dir = global->player->dir;
	new_dir.x = old_dir.x * cosf(angle) - old_dir.y * sinf(angle);
	new_dir.y = old_dir.x * sinf(angle) + old_dir.y * cosf(angle);
	global->player->dir = new_dir;
	angle_increment = FOV / (global->img->width - 1);
	i = 0;
	while (i < (int)global->img->width)
	{
		new_angle = (atan2f(new_dir.y, new_dir.x) - (float)(FOV / 2.0f))
			+ ((float)i * angle_increment);
		global->player->rays[i].direction = (t_vec2d){cosf(new_angle),
			sinf(new_angle)};
		i++;
	}
}
