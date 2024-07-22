/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   showMap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 11:18:22 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 15:08:37 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	draw_fov_lines(t_global *global)
{
	float		angle;
	t_player	*player;
	t_vec2d		start;
	t_vec2d		left_end;
	t_vec2d		right_end;

	player = global->player;
	start = player->pos;
	angle = atan2f(player->dir.y, player->dir.x) - (float)(FOV / 2);
	left_end.x = start.x + cosf(angle) * 10;
	left_end.y = start.y + sinf(angle) * 10;
	angle = atan2f(player->dir.y, player->dir.x) + (float)(FOV / 2);
	right_end.x = start.x + cosf(angle) * 10;
	right_end.y = start.y + sinf(angle) * 10;
	draw_line(global, start, left_end, get_rgba(255, 255, 255, 255));
	draw_line(global, start, right_end, get_rgba(255, 255, 255, 255));
}

void	draw_game_elements(t_global *global)
{
	int	i;
	int	j;

	i = 0;
	while ((uint32_t)i < global->img->width)
	{
		if (SHOW_RAYS)
			draw_ray(global, &global->player->rays[i]);
		j = 0;
		while (j < global->player->rays[i].collision_count)
		{
			if (SHOW_COLLISIONS)
				draw_circle(global,
					&(t_circle){global->player->rays[i].collisions[j].point, 3},
					get_rgba(0, 255, 0, 255));
			j++;
		}
		if (SHOW_COLLISIONS)
			draw_circle(global,
				&(t_circle){global->player->rays[i].closest_collision->point,
				3}, get_rgba(0, 0, 255, 255));
		i++;
	}
	if (SHOW_FOV)
		draw_fov_lines(global);
}

void	show_map(t_global *global)
{
	int	i;

	i = 0;
	while (i < global->line_count)
	{
		draw_line(global, global->lines[i].a, global->lines[i].b, get_rgba(255,
				255, 255, 255));
		i++;
	}
	draw_circle(global, &(t_circle){global->player->pos, 5}, get_rgba(255, 0, 0,
			255));
	i = 0;
	while (i < (int)global->img->width)
	{
		if (global->player->rays[i].collisions)
			free(global->player->rays[i].collisions);
		global->player->rays[i].collisions = NULL;
		global->player->rays[i].collision_count = 0;
		i++;
	}
	raycast(global);
	draw_game_elements(global);
}
