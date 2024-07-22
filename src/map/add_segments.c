/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_segments.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:14:44 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 15:19:13 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	add_wall_segments(t_global *global, t_line **lines, int *line_count,
		t_add_segments *seg)
{
	if (!is_wall_above(global, seg->x, seg->y) && !is_door_above(global, seg->x,
			seg->y))
	{
		add_line_segment(lines, line_count, set_wall_segment(seg->top_left,
				seg->top_right, HORIZONTAL));
	}
	if (!is_wall_below(global, seg->x, seg->y) && !is_door_below(global, seg->x,
			seg->y))
	{
		add_line_segment(lines, line_count, set_wall_segment(seg->bottom_left,
				seg->bottom_right, HORIZONTAL));
	}
	if (!is_wall_left(global, seg->x, seg->y) && !is_door_left(global, seg->x,
			seg->y))
	{
		add_line_segment(lines, line_count, set_wall_segment(seg->top_left,
				seg->bottom_left, VERTICAL));
	}
	if (!is_wall_right(global, seg->x, seg->y) && !is_door_right(global, seg->x,
			seg->y))
	{
		add_line_segment(lines, line_count, set_wall_segment(seg->top_right,
				seg->bottom_right, VERTICAL));
	}
}

void	vertical_door_segment(t_line **lines, int *line_count,
		t_add_segments *seg)
{
	seg->left_middle = (t_vec2d){seg->x + 0.5, seg->y};
	seg->right_middle = (t_vec2d){seg->x + 0.5, seg->y + 1};
	add_line_segment(lines, line_count, set_door_segment(seg->left_middle,
			seg->right_middle, VERTICAL, seg->door));
	add_line_segment(lines, line_count, set_door_side_segment(seg->top_left,
			seg->top_right, HORIZONTAL, seg->door));
	add_line_segment(lines, line_count, set_door_side_segment(seg->bottom_left,
			seg->bottom_right, HORIZONTAL, seg->door));
}

void	horizontal_door_segment(t_line **lines, int *line_count,
		t_add_segments *seg)
{
	seg->left_middle = (t_vec2d){seg->x, seg->y + 0.5};
	seg->right_middle = (t_vec2d){seg->x + 1, seg->y + 0.5};
	add_line_segment(lines, line_count, set_door_segment(seg->left_middle,
			seg->right_middle, HORIZONTAL, seg->door));
	add_line_segment(lines, line_count, set_door_side_segment(seg->top_left,
			seg->bottom_left, VERTICAL, seg->door));
	add_line_segment(lines, line_count, set_door_side_segment(seg->top_right,
			seg->bottom_right, VERTICAL, seg->door));
}

void	add_door_segments(t_global *global, t_line **lines, int *line_count,
		t_add_segments *seg)
{
	seg->door = NULL;
	seg->i = 0;
	while (seg->i < global->door_count)
	{
		seg->scaled_x = seg->x * global->scale_factor;
		seg->scaled_y = seg->y * global->scale_factor;
		if (global->doors[seg->i].pos.x == seg->scaled_x
			&& global->doors[seg->i].pos.y == seg->scaled_y)
		{
			seg->door = &global->doors[seg->i];
			break ;
		}
		seg->i++;
	}
	if (!seg->door)
		ft_exit_free(global);
	if (is_wall_left(global, seg->x, seg->y) && is_wall_right(global, seg->x,
			seg->y))
	{
		horizontal_door_segment(lines, line_count, seg);
	}
	else
	{
		vertical_door_segment(lines, line_count, seg);
	}
}

void	map_to_line_segments(t_global *global, t_line **lines, int *line_count)
{
	t_add_segments	seg;

	*lines = NULL;
	*line_count = 0;
	seg.y = -1;
	while (++seg.y < global->map->height)
	{
		seg.x = 0;
		while (seg.x < global->map->width)
		{
			if (global->map->map[seg.y][seg.x] == '1'
				|| global->map->map[seg.y][seg.x] == 'D')
			{
				seg.top_left = (t_vec2d){seg.x, seg.y};
				seg.top_right = (t_vec2d){seg.x + 1, seg.y};
				seg.bottom_left = (t_vec2d){seg.x, seg.y + 1};
				seg.bottom_right = (t_vec2d){seg.x + 1, seg.y + 1};
				if (global->map->map[seg.y][seg.x] == '1')
					add_wall_segments(global, lines, line_count, &seg);
				else
					add_door_segments(global, lines, line_count, &seg);
			}
			seg.x++;
		}
	}
}
