#include "../../inc/cub3d.h"

void	make_background_transparent(t_global *global)
{
	uint32_t	x;
	uint32_t	y;

	y = 0;
	while (y < global->minimap->height)
	{
		x = 0;
		while (x < global->minimap->width)
		{
			mlx_put_pixel(global->minimap, x, y, get_rgba(0, 0, 0, 100));
			x++;
		}
		y++;
	}
}

void	show_sky_and_floor(t_global *global)
{
	int	x;
	int	y;

	y = 0;
	while (y < global->window_height / 2)
	{
		x = 0;
		while (x < global->window_width)
		{
			mlx_put_pixel(global->img, x, y, get_rgba(0, 255, 255, 255));
			x++;
		}
		y++;
	}
	while (y < global->window_height)
	{
		x = 0;
		while (x < global->window_width)
		{
			mlx_put_pixel(global->img, x, y, get_rgba(255, 255, 0, 255));
			x++;
		}
		y++;
	}
}

float	map_distance_to_height(float distance, t_global *global)
{
	float	height;
	float		window_height;

	window_height = (float)global->window_height;
	if (distance == 0)
		height = window_height;
	else
		height = (global->scale_factor / distance) * window_height;
	return (height);
}

float	get_distance(t_vec2d a, t_vec2d b)
{
	return (sqrtf(powf(b.x - a.x, 2) + powf(b.y - a.y, 2)));
}

double point_line_distance(t_vec2d point, t_line *line);
t_collision	*new_collision(t_collision *collisions, int *collision_count,
		t_vec2d point, t_line *line, t_face face);

void	check_inactive_lines(t_global *global)
{
	float distance;
	t_ray tmp_ray;
	t_collision *tmp;
	t_vec2d intersection;
	t_face face;
	float min_distance = 1000000;

	tmp_ray.origin = global->player->pos;
	tmp_ray.direction = global->player->dir;
	tmp_ray.collisions = NULL;
	tmp_ray.collision_count = 0;

	for (int i = 0; i < global->line_count; i++)
	{
		intersection = ray_line_collision(&tmp_ray, &global->lines[i], &face);
		if (intersection.x != -1)
		{
			tmp = new_collision(tmp_ray.collisions, &tmp_ray.collision_count, intersection, &global->lines[i], face);
			if (!tmp)
				return ;
			tmp_ray.collisions = tmp;
		}
	}
	for (int i = 0; i < tmp_ray.collision_count; i++)
	{
		distance = get_distance(global->player->pos, tmp_ray.collisions[i].point);
		if (distance < min_distance)
		{
			min_distance = distance;
			tmp_ray.closest_collision = &tmp_ray.collisions[i];
		}
	}
	for (int i = 0; i < global->door_count; i++)
	{
		distance = point_line_distance(global->player->pos, global->door_line[i]);
		if (distance < INTERACT_DISTANCE && distance > 2.0 && (tmp_ray.closest_collision->line->type == DOOR || tmp_ray.closest_collision->line->type == DOOR_SIDE))
			global->door_line[i]->door->state = CLOSING;
	}
}

void	check_active_lines(t_global *global)
{
	float distance;
	t_ray tmp_ray;
	t_collision *tmp;
	t_vec2d intersection;
	t_face face;
	float min_distance = 1000000;

	tmp_ray.origin = global->player->pos;
	tmp_ray.direction = global->player->dir;
	tmp_ray.collisions = NULL;
	tmp_ray.collision_count = 0;

	for (int i = 0; i < global->line_count; i++)
	{
		intersection = ray_line_collision(&tmp_ray, &global->lines[i], &face);
		if (intersection.x != -1)
		{
			tmp = new_collision(tmp_ray.collisions, &tmp_ray.collision_count, intersection, &global->lines[i], face);
			if (!tmp)
				return ;
			tmp_ray.collisions = tmp;
		}
	}
	for (int i = 0; i < tmp_ray.collision_count; i++)
	{
		distance = get_distance(global->player->pos, tmp_ray.collisions[i].point);
		if (distance < min_distance)
		{
			min_distance = distance;
			tmp_ray.closest_collision = &tmp_ray.collisions[i];
		}
	}
	for (int i = 0; i < global->door_count; i++)
	{
		distance = point_line_distance(global->player->pos, global->door_line[i]);
		if (distance < INTERACT_DISTANCE && distance > 2.0 && (tmp_ray.closest_collision->line->type == DOOR || tmp_ray.closest_collision->line->type == DOOR_SIDE))
			global->door_line[i]->door->state = OPENING;
	}
}

// void update_door_segments(t_global *global)
// {
// 	for (int i = 0; i < global->door_count; i++)
// 	{
// 		if (global->doors[i].state == OPENING)
// 		{
// 			if (global->door_line[i]->alignment == VERTICAL)
// 			{
// 				if (global->door_line[i]->a.y < global->door_line[i]->b.y)
// 				{
// 					global->door_line[i]->a.y += 0.1;
// 					global->door_line[i]->b.y -= 0.1;
// 				}
// 				else
// 				{
// 					global->door_line[i]->a.y -= 0.1;
// 					global->door_line[i]->b.y += 0.1;
// 				}
// 			}
// 			else
// 			{
// 				if (global->door_line[i]->a.x < global->door_line[i]->b.x)
// 				{
// 					global->door_line[i]->a.x += 0.1;
// 					global->door_line[i]->b.x -= 0.1;
// 				}
// 				else
// 				{
// 					global->door_line[i]->a.x -= 0.1;
// 					global->door_line[i]->b.x += 0.1;
// 				}
// 			}
// 			if (global->door_line[i]->a.y == global->door_line[i]->b.y && global->door_line[i]->a.x == global->door_line[i]->b.x)
// 			{
// 				global->doors[i].state = OPEN;
// 			}
// 		}
// 		else if (global->doors[i].state == CLOSING)
// 		{
// 			if (global->door_line[i]->alignment == VERTICAL)
// 			{
// 				if (global->door_line[i]->a.y < global->door_line[i]->b.y)
// 				{
// 					global->door_line[i]->a.y -= 0.1;
// 					global->door_line[i]->b.y += 0.1;
// 				}
// 				else
// 				{
// 					global->door_line[i]->a.y += 0.1;
// 					global->door_line[i]->b.y -= 0.1;
// 				}
// 			}
// 			else
// 			{
// 				if (global->door_line[i]->a.x < global->door_line[i]->b.x)
// 				{
// 					global->door_line[i]->a.x -= 0.1;
// 					global->door_line[i]->b.x += 0.1;
// 				}
// 				else
// 				{
// 					global->door_line[i]->a.x += 0.1;
// 					global->door_line[i]->b.x -= 0.1;
// 				}
// 			}
// 			if (global->door_line[i]->a.y == global->door_line[i]->b.y && global->door_line[i]->a.x == global->door_line[i]->b.x)
// 			{
// 				global->doors[i].state = CLOSED;
// 			}
// 		}
// 	}
// }

void update_door_segments(t_global *global)
{
	float scaled_x;
	float scaled_y;

	for (int i = 0; i < global->door_count; i++)
	{
		if (global->doors[i].state == OPENING)
		{

			if (global->door_line[i]->alignment == VERTICAL)
			{
				scaled_y = global->door_line[i]->a.y / global->scale_factor;
				if (scaled_y > global->door_line[i]->open_end.y - 2)
				{
					global->door_line[i]->a.y -= 0.1;
					global->door_line[i]->b.y -= 0.1;
				}
				if (scaled_y <= global->door_line[i]->open_end.y - 2)
					global->doors[i].state = OPEN;
			}
			else
			{
				scaled_x = global->door_line[i]->a.x / global->scale_factor;
				if (scaled_x < global->door_line[i]->open_end.x)
				{
					global->door_line[i]->a.x += 0.1;
					global->door_line[i]->b.x += 0.1;
				}
				if (scaled_x >= global->door_line[i]->open_end.x)
					global->doors[i].state = OPEN;
			}
		}
		else if (global->doors[i].state == CLOSING)
		{
			if (global->door_line[i]->alignment == VERTICAL)
			{
				scaled_y = global->door_line[i]->a.y / global->scale_factor;
				if (scaled_y < global->door_line[i]->close_end.y)
				{
					global->door_line[i]->a.y += 0.1;
					global->door_line[i]->b.y += 0.1;
				}
				if (scaled_y >= global->door_line[i]->close_end.y)
					global->doors[i].state = CLOSED;
			}
			else
			{
				scaled_x = global->door_line[i]->a.x / global->scale_factor;
				if (scaled_x > global->door_line[i]->close_end.x)
				{
					global->door_line[i]->a.x -= 0.1;
					global->door_line[i]->b.x -= 0.1;
				}
				if (scaled_x <= global->door_line[i]->close_end.x)
					global->doors[i].state = CLOSED;
			}
		}
	}
}

void	load_textures(mlx_texture_t **texture_north,
		mlx_texture_t **texture_south, mlx_texture_t **texture_east,
		mlx_texture_t **texture_west, mlx_texture_t **door)
{
	if (!*texture_north)
		*texture_north = mlx_load_png("textures/cobblestone.png");
	if (!*texture_south)
		*texture_south = mlx_load_png("textures/dirt.png");
	if (!*texture_east)
		*texture_east = mlx_load_png("textures/polished_granite.png");
	if (!*texture_west)
		*texture_west = mlx_load_png("textures/piston_bottom.png");
	if (!*door)
		*door = mlx_load_png("textures/diamond.png");
	if (!*texture_north || !*texture_south || !*texture_east || !*texture_west || !*door)
	{
		fprintf(stderr, "Error loading textures\n");
	}
}

mlx_texture_t	*select_texture(t_collision *collision,
		mlx_texture_t *texture_north, mlx_texture_t *texture_south,
		mlx_texture_t *texture_east, mlx_texture_t *texture_west, mlx_texture_t *door)
{
	if (collision->face == NORTH)
		return (texture_north);
	else if (collision->face == SOUTH)
		return (texture_south);
	else if (collision->face == EAST)
		return (texture_east);
	else if (collision->face == DOOR || collision->face == DOOR_SIDE)
		return (door);
	else
		return (texture_west);
}
float	calculate_hit_percentage(t_collision *collision)
{
	if (collision->line->alignment == VERTICAL)
	{
		return ((collision->point.y - collision->line->a.y)
			/ (collision->line->b.y - collision->line->a.y));
	}
	else
	{
		return ((collision->point.x - collision->line->a.x)
			/ (collision->line->b.x - collision->line->a.x));
	}
}

void	draw_column(t_global *global, int x, int top_y, int bar_height,
		mlx_texture_t *texture, float hit_percentage)
{
	uint8_t	*pixel;
    int i;
	int r;
    int g;
    int b;
    int color;
	int draw_y;
    int texture_x;
    int texture_y;

	texture_x = (int)(hit_percentage * (float)texture->width);
	i = 0;
	while (i < bar_height)
	{
		texture_y = (int)(((float)i / (float)bar_height) * (float)texture->height);
		texture_y = (int)fmax(fmin(texture_y, texture->height - 1), 0);
		texture_x = (int)fmax(fmin(texture_x, texture->width - 1), 0);
		pixel = &(texture->pixels[(texture_y * texture->width + texture_x)
				* texture->bytes_per_pixel]);
		if (pixel != NULL)
		{
			r = pixel[0];
			g = pixel[1];
			b = pixel[2];
			color = get_rgba(r, g, b, 255);
			draw_y = top_y + i;
			if (draw_y >= 0 && (uint32_t)draw_y < global->img->height)
			{
				if (x >= 0 && (uint32_t)x < global->img->width)
				{
					mlx_put_pixel(global->img, x, draw_y, color);
				}
			}
		}
		i++;
	}
}

double point_line_distance(t_vec2d point, t_line *line);
t_collision	*new_collision(t_collision *collisions, int *collision_count,
		t_vec2d point, t_line *line, t_face face);

void	check_inactive_lines(t_global *global)
{
	float distance;
	t_ray tmp_ray;
	t_collision *tmp;
	t_vec2d intersection;
	t_face face;
	float min_distance = 1000000;

	tmp_ray.origin = global->player->pos;
	tmp_ray.direction = global->player->dir;
	tmp_ray.collisions = NULL;
	tmp_ray.collision_count = 0;

	for (int i = 0; i < global->line_count; i++)
	{
		intersection = ray_line_collision(&tmp_ray, &global->lines[i], &face);
		if (intersection.x != -1)
		{
			tmp = new_collision(tmp_ray.collisions, &tmp_ray.collision_count, intersection, &global->lines[i], face);
			if (!tmp)
				return ;
			tmp_ray.collisions = tmp;
		}
	}
	for (int i = 0; i < tmp_ray.collision_count; i++)
	{
		distance = get_distance(global->player->pos, tmp_ray.collisions[i].point);
		if (distance < min_distance)
		{
			min_distance = distance;
			tmp_ray.closest_collision = &tmp_ray.collisions[i];
		}
	}
	for (int i = 0; i < global->door_count; i++)
	{
		distance = point_line_distance(global->player->pos, global->door_line[i]);
		if (distance < INTERACT_DISTANCE && distance > 2.0 && (tmp_ray.closest_collision->line->type == DOOR || tmp_ray.closest_collision->line->type == DOOR_SIDE))
			global->door_line[i]->door->state = CLOSING;
	}
}

void	check_active_lines(t_global *global)
{
	float distance;
	t_ray tmp_ray;
	t_collision *tmp;
	t_vec2d intersection;
	t_face face;
	float min_distance = 1000000;

	tmp_ray.origin = global->player->pos;
	tmp_ray.direction = global->player->dir;
	tmp_ray.collisions = NULL;
	tmp_ray.collision_count = 0;

	for (int i = 0; i < global->line_count; i++)
	{
		intersection = ray_line_collision(&tmp_ray, &global->lines[i], &face);
		if (intersection.x != -1)
		{
			tmp = new_collision(tmp_ray.collisions, &tmp_ray.collision_count, intersection, &global->lines[i], face);
			if (!tmp)
				return ;
			tmp_ray.collisions = tmp;
		}
	}
	for (int i = 0; i < tmp_ray.collision_count; i++)
	{
		distance = get_distance(global->player->pos, tmp_ray.collisions[i].point);
		if (distance < min_distance)
		{
			min_distance = distance;
			tmp_ray.closest_collision = &tmp_ray.collisions[i];
		}
	}
	for (int i = 0; i < global->door_count; i++)
	{
		distance = point_line_distance(global->player->pos, global->door_line[i]);
		if (distance < INTERACT_DISTANCE && distance > 2.0 && (tmp_ray.closest_collision->line->type == DOOR || tmp_ray.closest_collision->line->type == DOOR_SIDE))
			global->door_line[i]->door->state = OPENING;
	}
}

// void update_door_segments(t_global *global)
// {
// 	for (int i = 0; i < global->door_count; i++)
// 	{
// 		if (global->doors[i].state == OPENING)
// 		{
// 			if (global->door_line[i]->alignment == VERTICAL)
// 			{
// 				if (global->door_line[i]->a.y < global->door_line[i]->b.y)
// 				{
// 					global->door_line[i]->a.y += 0.1;
// 					global->door_line[i]->b.y -= 0.1;
// 				}
// 				else
// 				{
// 					global->door_line[i]->a.y -= 0.1;
// 					global->door_line[i]->b.y += 0.1;
// 				}
// 			}
// 			else
// 			{
// 				if (global->door_line[i]->a.x < global->door_line[i]->b.x)
// 				{
// 					global->door_line[i]->a.x += 0.1;
// 					global->door_line[i]->b.x -= 0.1;
// 				}
// 				else
// 				{
// 					global->door_line[i]->a.x -= 0.1;
// 					global->door_line[i]->b.x += 0.1;
// 				}
// 			}
// 			if (global->door_line[i]->a.y == global->door_line[i]->b.y && global->door_line[i]->a.x == global->door_line[i]->b.x)
// 			{
// 				global->doors[i].state = OPEN;
// 			}
// 		}
// 		else if (global->doors[i].state == CLOSING)
// 		{
// 			if (global->door_line[i]->alignment == VERTICAL)
// 			{
// 				if (global->door_line[i]->a.y < global->door_line[i]->b.y)
// 				{
// 					global->door_line[i]->a.y -= 0.1;
// 					global->door_line[i]->b.y += 0.1;
// 				}
// 				else
// 				{
// 					global->door_line[i]->a.y += 0.1;
// 					global->door_line[i]->b.y -= 0.1;
// 				}
// 			}
// 			else
// 			{
// 				if (global->door_line[i]->a.x < global->door_line[i]->b.x)
// 				{
// 					global->door_line[i]->a.x -= 0.1;
// 					global->door_line[i]->b.x += 0.1;
// 				}
// 				else
// 				{
// 					global->door_line[i]->a.x += 0.1;
// 					global->door_line[i]->b.x -= 0.1;
// 				}
// 			}
// 			if (global->door_line[i]->a.y == global->door_line[i]->b.y && global->door_line[i]->a.x == global->door_line[i]->b.x)
// 			{
// 				global->doors[i].state = CLOSED;
// 			}
// 		}
// 	}
// }

void update_door_segments(t_global *global)
{
	float scaled_x;
	float scaled_y;

	for (int i = 0; i < global->door_count; i++)
	{
		if (global->doors[i].state == OPENING)
		{

			if (global->door_line[i]->alignment == VERTICAL)
			{
				scaled_y = global->door_line[i]->a.y / global->scale_factor;
				if (scaled_y > global->door_line[i]->open_end.y - 2)
				{
					global->door_line[i]->a.y -= 0.1;
					global->door_line[i]->b.y -= 0.1;
				}
				if (scaled_y <= global->door_line[i]->open_end.y - 2)
					global->doors[i].state = OPEN;
			}
			else
			{
				scaled_x = global->door_line[i]->a.x / global->scale_factor;
				if (scaled_x < global->door_line[i]->open_end.x)
				{
					global->door_line[i]->a.x += 0.1;
					global->door_line[i]->b.x += 0.1;
				}
				if (scaled_x >= global->door_line[i]->open_end.x)
					global->doors[i].state = OPEN;
			}
		}
		else if (global->doors[i].state == CLOSING)
		{
			if (global->door_line[i]->alignment == VERTICAL)
			{
				scaled_y = global->door_line[i]->a.y / global->scale_factor;
				if (scaled_y < global->door_line[i]->close_end.y)
				{
					global->door_line[i]->a.y += 0.1;
					global->door_line[i]->b.y += 0.1;
				}
				if (scaled_y >= global->door_line[i]->close_end.y)
					global->doors[i].state = CLOSED;
			}
			else
			{
				scaled_x = global->door_line[i]->a.x / global->scale_factor;
				if (scaled_x > global->door_line[i]->close_end.x)
				{
					global->door_line[i]->a.x -= 0.1;
					global->door_line[i]->b.x -= 0.1;
				}
				if (scaled_x <= global->door_line[i]->close_end.x)
					global->doors[i].state = CLOSED;
			}
		}
	}
}

void	render_3d(t_global *global)
{
	static mlx_texture_t	*texture_north = NULL;
	static mlx_texture_t	*texture_south = NULL;
	static mlx_texture_t	*texture_east = NULL;
	static mlx_texture_t	*texture_west = NULL;
	static mlx_texture_t	*door = NULL;
	t_ray					*ray;
	t_collision				*collision;
	mlx_texture_t			*texture;
	int						i;

	float player_angle, ray_angle, angle_diff;
	float distance, perpendicular_distance, hit_percentage;
	int bar_width, bar_height, center_y, top_y, x;
	load_textures(&texture_north, &texture_south, &texture_east, &texture_west, &door);
	player_angle = atan2f(global->player->dir.y, global->player->dir.x);
	bar_width = 1;
	if (global->close)
		check_inactive_lines(global);
	if (global->open)
		check_active_lines(global);
	i = 0;
	while (i < (int)global->img->width)
	{
		ray = &global->player->rays[i];
		collision = ray->closest_collision;
		if (collision)
		{
			distance = get_distance(global->player->pos, collision->point);
			distance = fmaxf(distance, 0.1f);
			ray_angle = atan2f(ray->direction.y, ray->direction.x);
			angle_diff = ray_angle - player_angle;
			perpendicular_distance = distance * cosf(angle_diff);
			bar_height = (int)map_distance_to_height(perpendicular_distance, global);
			center_y = (int)global->img->height / 2;
			top_y = center_y - (bar_height / 2);
			x = i * bar_width;
			texture = select_texture(collision, texture_north, texture_south,
					texture_east, texture_west, door);
			hit_percentage = calculate_hit_percentage(collision);
			hit_percentage = fmaxf(fminf(hit_percentage, 1.0f), 0.0f);
			draw_column(global, x, top_y, bar_height, texture, hit_percentage);
		}
		i++;
	}
}
