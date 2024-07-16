#include "../../inc/cub3d.h"

void	get_denominator(float *denominator, t_ray *ray, t_line *line)
{
	*denominator = (ray->origin.x - (ray->origin.x + ray->direction.x))
		* (line->a.y - line->b.y) - (ray->origin.y - (ray->origin.y
				+ ray->direction.y)) * (line->a.x - line->b.x);
}
