#include "../../inc/cub3d.h"

void	get_denominator(float *denominator, t_ray *ray, t_line *line)
{
	*denominator = (ray->origin.x - (ray->origin.x + ray->direction.x))
		* (line->a.y - line->b.y) - (ray->origin.y - (ray->origin.y
				+ ray->direction.y)) * (line->a.x - line->b.x);
}

void	get_t_and_u(float *t, float *u, t_ray *ray, t_line *line,
		const float *denominator)
{
	*t = ((ray->origin.x - line->a.x) * (line->a.y - line->b.y) - (ray->origin.y
				- line->a.y) * (line->a.x - line->b.x)) / *denominator;
	*u = ((ray->origin.x - line->a.x) * (ray->origin.y - (ray->origin.y
					+ ray->direction.y)) - (ray->origin.y - line->a.y)
			* (ray->origin.x - (ray->origin.x + ray->direction.x)))
		/ *denominator;
}

void	set_faces(t_ray *ray, t_vec2d collision_point, t_face *face,
		t_line *line)
{
	if (line->alignment == HORIZONTAL)
	{
		if (ray->origin.y < collision_point.y)
			*face = SOUTH;
		else
			*face = NORTH;
	}
	else
	{
		if (ray->origin.x < collision_point.x)
			*face = EAST;
		else
			*face = WEST;
	}
}

t_vec2d	ray_line_collision(t_ray *ray, t_line *line, t_face *face)
{
	float	denominator;
	float	t;
	float	u;
	t_vec2d	collision_point;

	get_denominator(&denominator, ray, line);
	if (denominator == 0)
		return ((t_vec2d){-1, -1});
	get_t_and_u(&t, &u, ray, line, &denominator);
	if (t >= 0 && u >= 0 && u <= 1)
	{
		collision_point = (t_vec2d){ray->origin.x + t * ((ray->origin.x
					+ ray->direction.x) - ray->origin.x), ray->origin.y + t
			* ((ray->origin.y + ray->direction.y) - ray->origin.y)};
		set_faces(ray, collision_point, face, line);
		return (collision_point);
	}
	return ((t_vec2d){-1, -1});
}

t_collision	*new_collision(t_collision *collisions, int *collision_count,
		t_vec2d point, t_line *line, t_face face)
{
	t_collision	*new_collisions;

	new_collisions = realloc(collisions, (*collision_count + 1)
			* sizeof(t_collision));
	if (!new_collisions)
		return (NULL);
	new_collisions[*collision_count].point = point;
	new_collisions[*collision_count].line = line;
	new_collisions[*collision_count].face = face;
	(*collision_count)++;
	return (new_collisions);
}
