/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_segments_utils3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 14:30:51 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 14:31:21 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

t_segment	set_wall_segment(t_vec2d a, t_vec2d b, t_alignment alignment)
{
	t_segment	segment;

	segment.a = a;
	segment.b = b;
	segment.alignment = alignment;
	segment.type = WALL;
	segment.door = NULL;
	segment.open_end = (t_vec2d){-1, -1};
	segment.close_end = (t_vec2d){-1, -1};
	return (segment);
}

t_segment	set_door_side_segment(t_vec2d a, t_vec2d b, t_alignment alignment,
			t_door *door)
{
	t_segment	segment;

	segment.a = a;
	segment.b = b;
	segment.alignment = alignment;
	segment.type = DOOR_SIDE;
	segment.door = door;
	segment.open_end = (t_vec2d){-1, -1};
	segment.close_end = (t_vec2d){-1, -1};
	return (segment);
}

t_segment	set_door_segment(t_vec2d a, t_vec2d b, t_alignment alignment,
				t_door *door)
{
	t_segment	segment;

	segment.a = a;
	segment.b = b;
	segment.alignment = alignment;
	segment.type = DOOR;
	segment.door = door;
	segment.open_end = b;
	segment.close_end = a;
	return (segment);
}
