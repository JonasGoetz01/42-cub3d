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

static void	find_closest_collision(t_global *global, t_ray *ray)
{
	float		distance;
	float		min_distance;
	t_vec2d		player_pos;
	t_collision	*collisions;
	int			count;

	min_distance = 1000000;
	player_pos = global->player->pos;
	collisions = ray->collisions;
	count = ray->collision_count;
	global->i = 0;
	while (global->i < count)
	{
		distance = sqrtf(powf(player_pos.x - collisions[global->i].point.x, 2)
				+ powf(player_pos.y - collisions[global->i].point.y, 2));
		if (distance < min_distance)
		{
			min_distance = distance;
			ray->closest_collision = &collisions[global->i];
		}
		global->i++;
	}
}

