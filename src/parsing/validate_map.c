/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:26:41 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/17 11:20:23 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

bool	check_space_vicinity(char **map, int x, int y, int *height_width)
{
	if (y > 0 && map[y - 1][x] != '1' && map[y - 1][x] != ' ')
		return (false);
	if (y < height_width[0] - 1 && map[y + 1][x] != '1' && map[y + 1][x] != ' ')
		return (false);
	if (x > 0 && map[y][x - 1] != '1' && map[y][x - 1] != ' ')
		return (false);
	if (x < height_width[1] - 1 && map[y][x + 1] != '1' && map[y][x + 1] != ' ')
		return (false);
	return (true);
}

bool	valid_characters(char **map, int height)
{
	int	y;
	int	x;
	int	player;

	y = 0;
	player = 0;
	while (y < height)
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] != '0' && map[y][x] != '1' && map[y][x] != 'N'
				&& map[y][x] != 'S' && map[y][x] != 'E' && map[y][x] != 'W'
				&& map[y][x] != ' ' && map[y][x] != 'D')
				return (printf(ERR_CHAR), false);
			if (map[y][x] == 'N' || map[y][x] == 'S'
				|| map[y][x] == 'E' || map[y][x] == 'W')
				player++;
			x++;
		}
		y++;
	}
	if (player != 1)
		return (printf(ERR_PLAYER), false);
	return (true);
}

bool	vertical_door(char **map, int x, int y, int *height_width)
{
	if ((y > 0 && map[y - 1][x] != '1') || (y < height_width[0] - 1
		&& map[y + 1][x] != '1'))
		return (false);
	if ((x > 0 && (map[y][x - 1] != ' ' && map[y][x - 1] != '0'))
		|| (x < height_width[1] - 1 && (map[y][x + 1] != ' '
			&& map[y][x + 1] != '0')))
		return (false);
	return (true);
}

bool	horizontal_door(char **map, int x, int y, int *height_width)
{
	if ((x > 0 && map[y][x - 1] != '1') || (x < height_width[1] - 1
		&& map[y][x + 1] != '1'))
		return (false);
	if ((y > 0 && (map[y - 1][x] != ' ' && map[y - 1][x] != '0'))
		|| (y < height_width[0] - 1 && (map[y + 1][x] != ' '
			&& map[y + 1][x] != '0')))
		return (false);
	return (true);
}

bool	valid_map(char **map, int height)
{
	int	y;
	int	x;
	int	height_width[2];

	y = 0;
	height_width[0] = height;
	while (y < height)
	{
		height_width[1] = ft_strlen(map[y]);
		x = 0;
		while (x < height_width[1])
		{
			if (map[y][x] == ' '
				&& !check_space_vicinity(map, x, y, height_width))
				return (printf(ERR_MAP), false);
			if (map[y][x] == 'D' && !(vertical_door(map, x, y, height_width)
				|| horizontal_door(map, x, y, height_width)))
				return (printf(ERR_DOOR), false);
			x++;
		}
		y++;
	}
	if (!valid_characters(map, height))
		return (false);
	return (true);
}
