/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initMap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:26:41 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 11:42:42 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

t_vec2d	get_player_position(t_global *global)
{
	t_vec2d	player_pos;
	int		y;
	int		x;

	player_pos = (t_vec2d){0, 0};
	y = 0;
	while (y < global->map->height)
	{
		x = 0;
		while (x < global->map->width)
		{
			if (global->map->map[y][x] == 'N' || global->map->map[y][x] == 'E'
				|| global->map->map[y][x] == 'W'
				|| global->map->map[y][x] == 'S')
			{
				player_pos.x = x + 0.5;
				player_pos.y = y + 0.5;
				return (player_pos);
			}
			x++;
		}
		y++;
	}
	return (player_pos);
}

t_vec2d	get_player_direction(t_global *global)
{
	t_vec2d	player_pos;
	int		y;
	int		x;

	player_pos = (t_vec2d){0, 0};
	y = 0;
	while (y < global->map->height)
	{
		x = 0;
		while (x < global->map->width)
		{
			if (global->map->map[y][x] == 'N')
				return ((t_vec2d){0, -1});
			else if (global->map->map[y][x] == 'E')
				return ((t_vec2d){1, 0});
			else if (global->map->map[y][x] == 'W')
				return ((t_vec2d){-1, 0});
			else if (global->map->map[y][x] == 'S')
				return ((t_vec2d){0, 1});
			x++;
		}
		y++;
	}
	return (player_pos);
}

void	get_doors(t_global *global)
{
	int	count;
	int	y;
	int	x;

	count = 0;
	y = 0;
	while (y < global->map->height)
	{
		x = 0;
		while (x < global->map->width)
		{
			if (global->map->map[y][x] == 'D')
				count++;
			x++;
		}
		y++;
	}
	global->door_count = count;
	global->doors = malloc(sizeof(t_door) * count);
	count = 0;
	y = 0;
	while (y < global->map->height)
	{
		x = 0;
		while (x < global->map->width)
		{
			if (global->map->map[y][x] == 'D')
			{
				global->doors[count].pos.x = x * global->scale_factor;
				global->doors[count].pos.y = y * global->scale_factor;
				global->doors[count].state = CLOSED;
				count++;
			}
			x++;
		}
		y++;
	}
}
