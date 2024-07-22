/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   showMap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:26:41 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 11:42:35 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

bool	isWallAbove(t_global *global, int x, int y)
{
	if (y == 0)
		return (false);
	if (global->map->map[y - 1][x] == '1')
		return (true);
	return (false);
}

bool	isWallBelow(t_global *global, int x, int y)
{
	if (y == global->map->height - 1)
		return (false);
	if (global->map->map[y + 1][x] == '1')
		return (true);
	return (false);
}

bool	isWallLeft(t_global *global, int x, int y)
{
	if (x == 0)
		return (false);
	if (global->map->map[y][x - 1] == '1')
		return (true);
	return (false);
}

bool	isWallRight(t_global *global, int x, int y)
{
	if (x == global->map->width - 1)
		return (false);
	if (global->map->map[y][x + 1] == '1')
		return (true);
	return (false);
}

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

void	add_line_segment(t_line **lines, int *count, t_vec2d a, t_vec2d b,
		t_alignment alignment, t_type type, t_door *door, t_vec2d open_end,
		t_vec2d close_end)
{
	*lines = ft_realloc(*lines, *count * sizeof(t_line), (*count + 1)
			* sizeof(t_line));
	(*lines)[*count].a = a;
	(*lines)[*count].b = b;
	(*lines)[*count].alignment = alignment;
	(*lines)[*count].type = type;
	(*lines)[*count].door = door;
	(*lines)[*count].open_end = open_end;
	(*lines)[*count].close_end = close_end;
	(*count)++;
}

void	map_to_line_segments(t_global *global, t_line **lines, int *line_count)
{
	t_vec2d	top_left;
	t_vec2d	top_right;
	t_vec2d	bottom_left;
	t_vec2d	bottom_right;
	t_vec2d	left_middle;
	t_vec2d	right_middle;
	t_door	*door;
	float	scaled_x;
	float	scaled_y;
	int		i;

	int x, y;
	*lines = NULL;
	*line_count = 0;
	y = 0;
	while (y < global->map->height)
	{
		x = 0;
		while (x < global->map->width)
		{
			if (global->map->map[y][x] == '1' || global->map->map[y][x] == 'D')
			{
				top_left = (t_vec2d){x, y};
				top_right = (t_vec2d){x + 1, y};
				bottom_left = (t_vec2d){x, y + 1};
				bottom_right = (t_vec2d){x + 1, y + 1};
				if (global->map->map[y][x] == '1')
				{
					if (!isWallAbove(global, x, y) && !is_door_above(global, x,
							y))
					{
						add_line_segment(lines, line_count, top_left, top_right,
							HORIZONTAL, WALL, NULL, (t_vec2d){-1, -1},
							(t_vec2d){-1, -1});
					}
					if (!isWallBelow(global, x, y) && !is_door_below(global, x,
							y))
					{
						add_line_segment(lines, line_count, bottom_left,
							bottom_right, HORIZONTAL, WALL, NULL, (t_vec2d){-1,
							-1}, (t_vec2d){-1, -1});
					}
					if (!isWallLeft(global, x, y) && !is_door_left(global, x,
							y))
					{
						add_line_segment(lines, line_count, top_left,
							bottom_left, VERTICAL, WALL, NULL, (t_vec2d){-1,
							-1}, (t_vec2d){-1, -1});
					}
					if (!isWallRight(global, x, y) && !is_door_right(global, x,
							y))
					{
						add_line_segment(lines, line_count, top_right,
							bottom_right, VERTICAL, WALL, NULL, (t_vec2d){-1,
							-1}, (t_vec2d){-1, -1});
					}
				}
				else
				{
					door = NULL;
					i = 0;
					while (i < global->door_count)
					{
						scaled_x = x * global->scale_factor;
						scaled_y = y * global->scale_factor;
						if (global->doors[i].pos.x == scaled_x
							&& global->doors[i].pos.y == scaled_y)
						{
							door = &global->doors[i];
							break ;
						}
						i++;
					}
					if (!door)
						ft_exit_free(global);
					if (isWallLeft(global, x, y) && isWallRight(global, x, y))
					{
						left_middle = (t_vec2d){x, y + 0.5};
						right_middle = (t_vec2d){x + 1, y + 0.5};
						add_line_segment(lines, line_count, left_middle,
							right_middle, HORIZONTAL, DOOR, door, right_middle,
							left_middle);
						add_line_segment(lines, line_count, top_left,
							bottom_left, VERTICAL, DOOR_SIDE, door,
							(t_vec2d){-1, -1}, (t_vec2d){-1, -1});
						add_line_segment(lines, line_count, top_right,
							bottom_right, VERTICAL, DOOR_SIDE, door,
							(t_vec2d){-1, -1}, (t_vec2d){-1, -1});
					}
					else
					{
						left_middle = (t_vec2d){x + 0.5, y};
						right_middle = (t_vec2d){x + 0.5, y + 1};
						add_line_segment(lines, line_count, left_middle,
							right_middle, VERTICAL, DOOR, door, right_middle,
							left_middle);
						add_line_segment(lines, line_count, top_left, top_right,
							HORIZONTAL, DOOR_SIDE, door, (t_vec2d){-1, -1},
							(t_vec2d){-1, -1});
						add_line_segment(lines, line_count, bottom_left,
							bottom_right, HORIZONTAL, DOOR_SIDE, door,
							(t_vec2d){-1, -1}, (t_vec2d){-1, -1});
					}
				}
			}
			x++;
		}
		y++;
	}
}

void	draw_fov_lines(t_global *global)
{
	float		angle;
	t_player	*player;

	t_vec2d start, left_end, right_end;
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

void	showMap(t_global *global)
{
	int	i;
	int	j;

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
		{
			free(global->player->rays[i].collisions);
		}
		global->player->rays[i].collisions = NULL;
		global->player->rays[i].collision_count = 0;
		i++;
	}
	raycast(global);
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
