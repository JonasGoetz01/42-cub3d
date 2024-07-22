/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_segments_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:20:00 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 13:20:59 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

bool	is_door_left(t_global *global, int x, int y)
{
	if (x == 0)
		return (false);
	if (global->map->map[y][x - 1] == 'D')
		return (true);
	return (false);
}

bool	is_door_right(t_global *global, int x, int y)
{
	if (x == global->map->width - 1)
		return (false);
	if (global->map->map[y][x + 1] == 'D')
		return (true);
	return (false);
}

bool	is_door_above(t_global *global, int x, int y)
{
	if (y == 0)
		return (false);
	if (global->map->map[y - 1][x] == 'D')
		return (true);
	return (false);
}

bool	is_door_below(t_global *global, int x, int y)
{
	if (y == global->map->height - 1)
		return (false);
	if (global->map->map[y + 1][x] == 'D')
		return (true);
	return (false);
}
