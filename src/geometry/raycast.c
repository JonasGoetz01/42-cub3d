#include "../../inc/cub3d.h"

void	get_denominator(float *denominator, t_ray *ray, t_line *line)
{
	*denominator = (ray->origin.x - (ray->origin.x + ray->direction.x))
		* (line->a.y - line->b.y) - (ray->origin.y - (ray->origin.y
				+ ray->direction.y)) * (line->a.x - line->b.x);
}

void	get_t_and_u(float *t, float *u, t_ray *ray, t_line *line,
		float *denominator)
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

