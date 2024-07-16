#include "../../inc/cub3d.h"

void		get_denominator(float *denominator, t_ray *ray, t_line *line);
void		get_t_and_u(float *t, float *u, t_ray *ray, t_line *line,
				float *denominator);
void		set_faces(t_ray *ray, t_vec2d collision_point, t_face *face,
				t_line *line);
t_vec2d		ray_line_collision(t_ray *ray, t_line *line, t_face *face);
t_collision	*new_collision(t_collision *collisions, int *collision_count,
				t_vec2d point, t_line *line, t_face face);

static void	add_collision(t_ray *ray, t_vec2d intersection, t_line *line,
		t_face face)
{
	t_collision	*new_collisions;

	new_collisions = new_collision(ray->collisions, &ray->collision_count,
			intersection, line, face);
	if (new_collisions)
		ray->collisions = new_collisions;
}
