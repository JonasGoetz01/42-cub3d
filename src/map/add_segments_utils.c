/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_segments_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:16:42 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 14:30:25 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

bool	is_wall_above(t_global *global, int x, int y)
{
	if (y == 0)
		return (false);
	if (global->map->map[y - 1][x] == '1')
		return (true);
	return (false);
}

bool	is_wall_below(t_global *global, int x, int y)
{
	if (y == global->map->height - 1)
		return (false);
	if (global->map->map[y + 1][x] == '1')
		return (true);
	return (false);
}

bool	is_wall_left(t_global *global, int x, int y)
{
	if (x == 0)
		return (false);
	if (global->map->map[y][x - 1] == '1')
		return (true);
	return (false);
}

bool	is_wall_right(t_global *global, int x, int y)
{
	if (x == global->map->width - 1)
		return (false);
	if (global->map->map[y][x + 1] == '1')
		return (true);
	return (false);
}

void	add_line_segment(t_line **lines, int *count, t_segment segment)
{
	*lines = ft_realloc(*lines, *count * sizeof(t_line), (*count + 1)
			* sizeof(t_line));
	(*lines)[*count].a = segment.a;
	(*lines)[*count].b = segment.b;
	(*lines)[*count].alignment = segment.alignment;
	(*lines)[*count].type = segment.type;
	(*lines)[*count].door = segment.door;
	(*lines)[*count].open_end = segment.open_end;
	(*lines)[*count].close_end = segment.close_end;
	(*count)++;
}
