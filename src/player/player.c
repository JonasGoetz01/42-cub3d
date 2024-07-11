#include "../../inc/cub3d.h"

t_player	*new_player(t_global *global, t_vec2d pos, t_vec2d dir)
{
	t_player	*player;
	float		angle_increment;
	float		ray_angle;
	int			i;
	float		offset_distance;

	offset_distance = 30.0f;
	player = malloc(sizeof(t_player));
	if (!player)
		return (NULL);
	player->pos = pos;
	player->dir = dir;
	player->rays = malloc(sizeof(t_ray) * global->img->width);
	if (!player->rays)
	{
		free(player);
		return (NULL);
	}
	angle_increment = FOV / (global->img->width - 1);
	for (i = 0; i < (int)global->img->width; i++)
	{
		ray_angle = atan2f(player->dir.y, player->dir.x) - (FOV / 2.0f) + (i
				* angle_increment);
		player->rays[i].origin = (t_vec2d){player->pos.x - dir.x
			* offset_distance, player->pos.y - dir.y * offset_distance};
		// Apply the offset
		player->rays[i].direction = (t_vec2d){cosf(ray_angle), sinf(ray_angle)};
		player->rays[i].collisions = NULL;
		player->rays[i].collision_count = 0;
	}
	player->opponent_rays = malloc(sizeof(t_ray) * global->opponent_count);
	for (i = 0; i < global->opponent_count; i++)
	{
		global->opponent[i].visible = false;
		global->opponent[i].dead = false;
		ray_angle = atan2f(global->opponent[i].pos.y - player->pos.y,
				global->opponent[i].pos.x - player->pos.x);
		player->opponent_rays[i].origin = (t_vec2d){player->pos.x - dir.x
			* offset_distance, player->pos.y - dir.y * offset_distance};
		// Apply the offset
		player->opponent_rays[i].direction = (t_vec2d){cosf(ray_angle),
			sinf(ray_angle)};
		player->opponent_rays[i].collisions = NULL;
		player->opponent_rays[i].collision_count = 0;
	}
	return (player);
}

t_vec2d	calculate_perpendicular_direction(t_line dir_line, t_line wall)
{
	t_vec2d	wall_dir;
	t_vec2d	perpendicular;
	t_vec2d	dir_vector;
	t_vec2d	new_dir;
	float	dot_product;
	float	length;

	wall_dir.x = wall.b.x - wall.a.x;
	wall_dir.y = wall.b.y - wall.a.y;
	perpendicular.x = -wall_dir.y;
	perpendicular.y = wall_dir.x;
	length = sqrt(perpendicular.x * perpendicular.x + perpendicular.y
			* perpendicular.y);
	perpendicular.x /= length;
	perpendicular.y /= length;
	dir_vector.x = dir_line.b.x - dir_line.a.x;
	dir_vector.y = dir_line.b.y - dir_line.a.y;
	dot_product = dir_vector.x * perpendicular.x + dir_vector.y
		* perpendicular.y;
	new_dir.x = dot_product * perpendicular.x;
	new_dir.y = dot_product * perpendicular.y;
	return (new_dir);
}

bool	line_line_collision(t_line *a, t_line *b)
{
	float	x1;
	float	y1;
	float	x2;
	float	y2;
	float	x3;
	float	y3;
	float	x4;
	float	y4;
	float	denominator;
	float	ua;
	float	ub;

	x1 = a->a.x;
	y1 = a->a.y;
	x2 = a->b.x;
	y2 = a->b.y;
	x3 = b->a.x;
	y3 = b->a.y;
	x4 = b->b.x;
	y4 = b->b.y;
	denominator = ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	if (denominator == 0)
	{
		return (false);
	}
	ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denominator;
	ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denominator;
	return (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1);
}

#include <math.h> // Ensure math.h is included for sqrt and pow functions

bool	circle_line_collision(t_vec2d circle_center, float radius, t_line line)
{
	t_vec2d	line_vec;
	t_vec2d	closest_point;
	float	line_length;
	float	t;
	float	distance_squared;

	line_vec.x = line.b.x - line.a.x;
	line_vec.y = line.b.y - line.a.y;
	line_length = sqrt(line_vec.x * line_vec.x + line_vec.y * line_vec.y);
	if (line_length == 0)
		return (false);
	t = ((circle_center.x - line.a.x) * line_vec.x + (circle_center.y
				- line.a.y) * line_vec.y) / (line_length * line_length);
	t = fmax(0, fmin(1, t));
	closest_point.x = line.a.x + t * line_vec.x;
	closest_point.y = line.a.y + t * line_vec.y;
	distance_squared = pow(circle_center.x - closest_point.x, 2)
		+ pow(circle_center.y - closest_point.y, 2);
	return (distance_squared <= (radius * radius));
}

void	update_position(t_global *global, t_vec2d dir, float speed)
{
	t_vec2d	new_pos;
	bool	collision;
	float	base_angle;
	float	angle;

	collision = false;
	new_pos.x = global->player->pos.x + dir.x * speed * global->minimap_scale;
	new_pos.y = global->player->pos.y + dir.y * speed * global->minimap_scale;
	// Check collision with each wall
	for (int i = 0; i < global->line_count; i++)
	{
		if (circle_line_collision(new_pos, PLAYER_RADIUS * global->scale_factor,
				global->lines[i]))
		{
			collision = true;
			break ;
		}
	}
	// Update player position if no collision
	if (!collision)
	{
		global->player->pos = new_pos;
	}
	// Update ray origins
	for (int i = 0; i < (int)global->img->width; i++)
	{
		global->player->rays[i].origin = global->player->pos;
	}
	for (int i = 0; i < global->opponent_count; i++)
	{
		global->player->opponent_rays[i].origin = global->player->pos;
		global->player->opponent_rays[i].direction = (t_vec2d){cosf(atan2f(global->opponent[i].pos.y
					- global->player->pos.y, global->opponent[i].pos.x
					- global->player->pos.x)),
			sinf(atan2f(global->opponent[i].pos.y - global->player->pos.y,
					global->opponent[i].pos.x - global->player->pos.x))};
	}
	base_angle = atan2f(global->player->dir.y, global->player->dir.x) - (FOV
			/ 2.0f);
	for (int i = 0; i < global->opponent_count; i++)
	{
		angle = atan2f(global->opponent[i].pos.y - global->player->pos.y,
				global->opponent[i].pos.x - global->player->pos.x);
		if (angle < base_angle || angle > base_angle + FOV)
		{
			global->player->opponent_rays[i].direction = (t_vec2d){0, 0};
		}
	}
	for (int j = 0; j < global->opponent_count; j++)
	{
		global->player->opponent_rays[j].closest_collision = NULL;
		global->player->opponent_rays[j].collisions = NULL;
	}
}

void	rotate_player(t_global *global, float angle)
{
	float	cos_angle;
	float	sin_angle;
	t_vec2d	old_dir;
	t_vec2d	new_dir;
	float	new_x;
	float	new_y;
	float	angle_increment;
	float	base_angle;
	float	ray_angle;

	cos_angle = cosf(angle);
	sin_angle = sinf(angle);
	old_dir = global->player->dir;
	new_x = old_dir.x * cos_angle - old_dir.y * sin_angle;
	new_y = old_dir.x * sin_angle + old_dir.y * cos_angle;
	new_dir = (t_vec2d){new_x, new_y};
	global->player->dir = new_dir;
	angle_increment = FOV / (global->img->width - 1);
	base_angle = atan2f(new_dir.y, new_dir.x) - (FOV / 2.0f);
	for (int i = 0; i < (int)global->img->width; i++)
	{
		ray_angle = base_angle + (i * angle_increment);
		global->player->rays[i].direction = (t_vec2d){cosf(ray_angle),
			sinf(ray_angle)};
	}
	// if the opponent ray is outside the FOV,
	// set the direction to the player direction
	for (int i = 0; i < global->opponent_count; i++)
	{
		angle = atan2f(global->opponent[i].pos.y - global->player->pos.y,
				global->opponent[i].pos.x - global->player->pos.x);
		if (angle < base_angle || angle > base_angle + FOV)
		{
			global->player->opponent_rays[i].direction = global->player->dir;
		}
	}
	for (int j = 0; j < global->opponent_count; j++)
	{
		global->player->opponent_rays[j].closest_collision = NULL;
		global->player->opponent_rays[j].collisions = NULL;
	}
}
