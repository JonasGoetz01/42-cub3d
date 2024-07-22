/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initMap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:26:41 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 13:46:52 by jgotz            ###   ########.fr       */
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

int	get_door_amount(t_global *global)
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
	return (count);
}

void	get_doors(t_global *global)
{
	int	y;
	int	x;
	int	count;

	global->door_count = get_door_amount(global);
	global->doors = malloc(sizeof(t_door) * global->door_count);
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
