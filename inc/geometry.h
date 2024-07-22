/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geometry.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 15:55:48 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 15:27:26 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GEOMETRY_H
# define GEOMETRY_H

typedef struct s_vec2d
{
	float		x;
	float		y;
}				t_vec2d;

typedef enum e_alignment
{
	VERTICAL,
	HORIZONTAL
}				t_alignment;

typedef enum e_face
{
	NORTH,
	SOUTH,
	EAST,
	WEST,
	DOORS,
}				t_face;

typedef enum e_type
{
	WALL,
	DOOR,
	DOOR_SIDE
}				t_type;

typedef enum e_state
{
	CLOSED,
	OPENING,
	OPEN,
	CLOSING
}				t_state;

typedef struct s_door
{
	t_vec2d		pos;
	t_state		state;
}				t_door;

typedef struct s_line
{
	t_vec2d		a;
	t_vec2d		b;
	t_vec2d		open_end;
	t_vec2d		close_end;
	t_alignment	alignment;
	t_type		type;
	t_door		*door;
}				t_line;

typedef struct s_collision
{
	t_vec2d		point;
	t_line		*line;
	t_face		face;
}				t_collision;

typedef struct s_ray
{
	t_vec2d		origin;
	t_vec2d		direction;
	t_collision	*collisions;
	int			collision_count;
	t_collision	*closest_collision;
}				t_ray;

typedef struct s_circle
{
	t_vec2d		center;
	float		radius;
}				t_circle;

#endif
