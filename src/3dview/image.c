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
	int		window_height;

	window_height = global->window_height;
	if (distance == 0)
		height = window_height;
	else
		height = (global->scale_factor / distance) * window_height;
	// if (height > window_height)
	// 	height = window_height;
	return (height);
}

float	get_distance(t_vec2d a, t_vec2d b)
{
	return (sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2)));
}

double point_line_distance(t_vec2d point, t_line *line);
t_collision	*new_collision(t_collision *collisions, int *collision_count,
		t_vec2d point, t_line *line, t_face face);
t_vec2d	ray_line_collision(t_ray *ray, t_line *line, t_face *face, t_global *global);

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

	global->check = true;
	for (int i = 0; i < global->line_count; i++)
	{
		intersection = ray_line_collision(&tmp_ray, &global->lines[i], &face, global);
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
		// printf("Distance to door %d: %f\n", i, distance);
		if (global->door_line[i]->flag == INACTIVE && distance < INTERACT_DISTANCE && distance > 2.0 && tmp_ray.closest_collision->line->type == DOOR)
		{
			global->door_line[i]->flag = ACTIVE;
			global->door_line[i]->door->state = CLOSING;
			global->door_line[i]->door->animation_progress = 1.0;
		}
	}
	global->check = false;
	if (global->player->door_ray->collisions)
		free(global->player->door_ray->collisions);
	global->player->door_ray->collisions = NULL;
	global->player->door_ray->collision_count = 0;
}

void	render_3d(t_global *global)
{
	int						bar_width;
	int						i;
	t_ray					*ray;
	t_collision				*closest_collision;
	float					distance;
	int						bar_height;
	int						center_y;
	int						top_y;
	int						bottom_y;
	int						x;
	float					hit_percentage;
	int						color;
	mlx_texture_t			*texture;
	uint8_t					*pixel;
	int						draw_y;
	int						texture_x;
	int						texture_y;
	float					z_buffer[global->img->width];
	float					perpendicular_distance;
	float					ray_angle;
	float					player_angle;
	float					angle_diff;
	static mlx_texture_t	*texture_north = NULL;
	static mlx_texture_t	*texture_south = NULL;
	static mlx_texture_t	*texture_east = NULL;
	static mlx_texture_t	*texture_west = NULL;
	static mlx_texture_t	*door = NULL;
	int						r;
	int						g;
	int						b;

	// Load textures once
	if (!texture_north)
		texture_north = mlx_load_png("textures/cobblestone.png");
	if (!texture_south)
		texture_south = mlx_load_png("textures/dirt.png");
	if (!texture_east)
		texture_east = mlx_load_png("textures/polished_granite.png");
	if (!texture_west)
		texture_west = mlx_load_png("textures/piston_bottom.png");
	if (!door)
		door = mlx_load_png("textures/diamond.png");
	// Check if textures are loaded successfully
	if (!texture_north || !texture_south || !texture_east || !texture_west || !door)
	{
		fprintf(stderr, "Error loading textures\n");
		return ;
	}
	player_angle = atan2(global->player->dir.y, global->player->dir.x);
	bar_width = 1;
	t_ray *door_ray = global->player->door_ray;
	t_collision *door_collision = door_ray->closest_collision;
	float door_distance = get_distance(global->player->pos, door_collision->point);
	if (global->close)
		check_inactive_lines(global);
	if (door_collision && door_distance < INTERACT_DISTANCE && door_distance > 2.0 && global->open && /*(*/door_collision->line->type == DOOR /*|| door_collision->line->type == DOOR_SIDE)*/)
	{
		// if (global->close)
		// {
		// 	if (door_collision->line->flag == INACTIVE)
		// 	{
		// 		door_collision->line->flag = ACTIVE;
		// 		door_collision->line->door->state = CLOSING;
		// 		door_collision->line->door->animation_progress = 1.0;
		// 	}

		// }
		// else
		// {
			door_collision->line->flag = INACTIVE;
			door_collision->line->door->state = OPENING;
			door_collision->line->door->animation_progress = 0.0;
		// }
	}
	for (i = 0; i < (int)global->img->width; i++)
	{
		ray = &global->player->rays[i];
		closest_collision = ray->closest_collision;
		if (closest_collision)
		{
			distance = get_distance(global->player->pos,
					closest_collision->point);
			distance = fmax(distance, 0.1f);
			ray_angle = atan2(ray->direction.y, ray->direction.x);
			angle_diff = ray_angle - player_angle;
			perpendicular_distance = distance * cos(angle_diff);
			bar_height = map_distance_to_height(perpendicular_distance, global);
			center_y = global->img->height / 2;
			top_y = center_y - (bar_height / 2);
			bottom_y = center_y + (bar_height / 2);
			x = i * bar_width;
			switch (closest_collision->face)
			{
			case NORTH:
				texture = texture_north;
				break ;
			case SOUTH:
				texture = texture_south;
				break ;
			case EAST:
				texture = texture_east;
				break ;
			case WEST:
				texture = texture_west;
				break ;
			case DOORS:
				texture = door;
				break ;
			default:
				texture = texture_north;
				break ;
			}
			if (closest_collision->line->alignment == VERTICAL)
			{
				hit_percentage = (closest_collision->point.y
						- closest_collision->line->a.y)
					/ (closest_collision->line->b.y
						- closest_collision->line->a.y);
			}
			else
			{
				hit_percentage = (closest_collision->point.x
						- closest_collision->line->a.x)
					/ (closest_collision->line->b.x
						- closest_collision->line->a.x);
			}
			hit_percentage = fmax(fmin(hit_percentage, 1.0f), 0.0f);
			// Clamp hit_percentage to [0, 1]
			texture_x = (int)(hit_percentage * (texture->width));
			for (int j = 0; j < bar_height; j++)
			{
				texture_y = (int)(((float)j / bar_height) * (texture->height));
				if ((uint32_t)texture_y >= texture->height)
					texture_y = texture->height - 1;
				if (texture_y < 0)
					texture_y = 0;
				if ((uint32_t)texture_x >= texture->width)
					texture_x = texture->width - 1;
				if (texture_x < 0)
					texture_x = 0;
				pixel = &(texture->pixels[(texture_y * texture->width
							+ texture_x) * texture->bytes_per_pixel]);
				if (pixel != NULL)
				{
					r = pixel[0];
					g = pixel[1];
					b = pixel[2];
					color = get_rgba(r, g, b, 255);
					draw_y = top_y + j;
					if (draw_y >= 0 && (uint32_t)draw_y < global->img->height)
					{
						if (x >= 0 && (uint32_t)x < global->img->width)
						{
							mlx_put_pixel(global->img, x, draw_y, color);
						}
					}
				}
			}
			z_buffer[i] = perpendicular_distance;
		}
	}
}
