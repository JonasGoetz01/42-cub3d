#include "../../inc/cub3d.h"

void	collision_helper(t_ray *ray, t_player *player, t_vec2d dir,
		float ray_angle)
{
	ray->origin = (t_vec2d){player->pos.x - dir.x, player->pos.y - dir.y};
	ray->direction = (t_vec2d){cosf(ray_angle), sinf(ray_angle)};
	ray->collisions = NULL;
	ray->collision_count = 0;
}

t_player	*new_player(t_global *global, t_vec2d pos, t_vec2d dir)
{
	t_player	*player;
	float		angle_increment;
	float		ray_angle;
	int			i;

	player = malloc(sizeof(t_player));
	if (!player)
		return (NULL);
	player->pos = pos;
	player->dir = dir;
	player->rays = malloc(sizeof(t_ray) * global->img->width);
	if (!player->rays)
		return (free(player), NULL);
	angle_increment = FOV / (global->img->width - 1);
	i = 0;
	while (i < (int)global->img->width)
	{
		ray_angle = atan2f(player->dir.y, player->dir.x) - (FOV / 2.0f) + (i
				* angle_increment);
		collision_helper(&(player->rays[i]), player, dir, ray_angle);
		i++;
	}
	return (player);
}

t_vec2d	calculate_perpendicular_direction(t_line dir_line, t_line wall)
{
	t_vec2d	wall_dir;
	t_vec2d	perpendicular;
	t_vec2d	dir_vector;
	t_vec2d	new_dir;
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
	new_dir.x = dir_vector.x * perpendicular.x + dir_vector.y * perpendicular.y
		* perpendicular.x;
	new_dir.y = dir_vector.x * perpendicular.x + dir_vector.y * perpendicular.y
		* perpendicular.y;
	return (new_dir);
}

bool	line_line_collision(t_line *a, t_line *b)
{
	float	denominator;
	float	ua;
	float	ub;

	denominator = ((b->b.y - b->a.y) * (a->b.x - a->a.x) - (b->b.x - b->a.x)
			* (a->b.y - a->a.y));
	if (denominator == 0)
		return (false);
	ua = ((b->b.x - b->a.x) * (a->a.y - b->a.y) - (b->b.y - b->a.y) * (a->a.x
				- b->a.x)) / denominator;
	ub = ((a->b.x - a->a.x) * (a->a.y - b->a.y) - (a->b.y - a->a.y) * (a->a.x
				- b->a.x)) / denominator;
	return (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1);
}

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
	t_vec2d		new_pos;
	bool		collision_x;
	bool		collision_y;
	t_vec2d		temp_pos;
	float		base_angle;
	static int	sprite_counter = 0;

	collision_x = false;
	collision_y = false;
	if (dir.x != 0 && dir.y != 0)
	{
		if (sprite_counter % 10 == 0)
		{
			global->sprite_index++;
			global->sprite_index %= 4;
			sprite_counter = 1;
		}
		sprite_counter++;
	}
	else
		global->sprite_index = 0;
	// Calculate new position
	new_pos.x = global->player->pos.x + dir.x * speed * global->minimap_scale;
	new_pos.y = global->player->pos.y + dir.y * speed * global->minimap_scale;
	// Check collision for x-axis
	temp_pos = global->player->pos;
	temp_pos.x = new_pos.x;
	for (int i = 0; i < global->line_count; i++)
	{
		if (circle_line_collision(temp_pos, PLAYER_RADIUS
				* global->scale_factor, global->lines[i]))
		{
			collision_x = true;
			break ;
		}
	}
	if (!collision_x)
		global->player->pos.x = new_pos.x;
	// Check collision for y-axis
	temp_pos = global->player->pos;
	temp_pos.y = new_pos.y;
	for (int i = 0; i < global->line_count; i++)
	{
		if (circle_line_collision(temp_pos, PLAYER_RADIUS
				* global->scale_factor, global->lines[i]))
		{
			collision_y = true;
			break ;
		}
	}
	if (!collision_y)
		global->player->pos.y = new_pos.y;
	// Update ray origins
	for (int i = 0; i < (int)global->img->width; i++)
		global->player->rays[i].origin = global->player->pos;
	base_angle = atan2f(global->player->dir.y, global->player->dir.x) - (FOV
			/ 2.0f);
}

void	rotate_player(t_global *global, float angle)
{
	t_vec2d	old_dir;
	t_vec2d	new_dir;
	float	angle_increment;
	float	base_angle;
	float	ray_angle;

	old_dir = global->player->dir;
	new_dir.x = old_dir.x * cosf(angle) - old_dir.y * sinf(angle);
	new_dir.y = old_dir.x * sinf(angle) + old_dir.y * cosf(angle);
	global->player->dir = new_dir;
	angle_increment = FOV / (global->img->width - 1);
	base_angle = atan2f(new_dir.y, new_dir.x) - (FOV / 2.0f);
	for (int i = 0; i < (int)global->img->width; i++)
	{
		ray_angle = base_angle + (i * angle_increment);
		global->player->rays[i].direction = (t_vec2d){cosf(ray_angle),
			sinf(ray_angle)};
	}
}
