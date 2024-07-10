/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:26:41 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/10 14:28:03 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

bool check_space_vicinity(char **map, int x, int y, int width, int height)
{
	if (y > 0 && map[y-1][x] != '1' && map[y-1][x] != ' ')
		return (false);
	if (y < height - 1 && map[y+1][x] != '1' && map[y+1][x] != ' ')
		return (false);
	if (x > 0 && map[y][x-1] != '1' && map[y][x-1] != ' ')
		return (false);
	if (x < width - 1 && map[y][x+1] != '1' && map[y][x+1] != ' ')
		return (false);
	return (true);
}

bool valid_characters(char **map, int height)
{
	int y;
	int x;
	int player;

	y = 0;
	player = 0;
	while (y < height)
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] != '0' && map[y][x] != '1' && map[y][x] != 'O' && map[y][x] != 'N'
				&& map[y][x] != 'S' && map[y][x] != 'E' && map[y][x] != 'W' && map[y][x] != ' ')
			{
				// error
				return (false);
			}
			if (map[y][x] == 'N' || map[y][x] == 'S' || map[y][x] == 'E' || map[y][x] == 'W')
				player++;
			if (player > 1)
			{
				// error
				return (false);
			}
			x++;
		}
		y++;
	}
	return (true);
}

bool valid_map(char **map, int height)
{
	int y;
	int x;
	int width;

	y = 0;
	while (y < height)
	{
		width = ft_strlen(map[y]);
		x = 0;
		while (x < width)
		{
			if (map[y][x] == ' ' && !check_space_vicinity(map, x, y, width, height))
				return (false);
			x++;
		}
		y++;
	}
	if (!valid_characters(map, height)) // implement
		return (false);
	return (true);
}
