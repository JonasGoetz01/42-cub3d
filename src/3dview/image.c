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

// @TODO sieht sehr scheiße aus, muss nochmal überarbeitet werden
void	show_sky_and_floor(t_global *global)
{
	int						x;
	int						y;
	static mlx_texture_t	*texture_sky = NULL;
	static mlx_texture_t	*texture_floor = NULL;
	int						texture_x;
	int						texture_y;
	uint8_t					*pixel;
	float					player_angle;
	float					sky_offset;
	float					floor_offset;
	int						sky_width;
	int						floor_width;
	float					texture_rotation_speed;

	texture_rotation_speed = 1.9f;
	if (!texture_sky)
		texture_sky = mlx_load_png("textures/sky.png");
	if (!texture_floor)
		texture_floor = mlx_load_png("textures/floor.png");
	if (!texture_sky || !texture_floor)
	{
		fprintf(stderr, "Error loading textures\n");
		return ;
	}
	player_angle = atan2(global->player->dir.y, global->player->dir.x);
	sky_width = texture_sky->width;
	floor_width = texture_floor->width;
	sky_offset = (player_angle / (2.0f * M_PI)) * sky_width
		* texture_rotation_speed;
	floor_offset = (player_angle / (2.0f * M_PI)) * floor_width
		* texture_rotation_speed;
	sky_offset = fmod(sky_offset + sky_width, sky_width);
	floor_offset = fmod(floor_offset + floor_width, floor_width);
	// Ensure positive offset
	y = 0;
	while (y < global->window_height / 2)
	{
		texture_y = (int)(((float)y / (global->window_height / 2))
				* texture_sky->height);
		x = 0;
		while (x < global->window_width)
		{
			texture_x = (int)(((float)(x + sky_offset) / global->window_width)
					* sky_width);
			texture_x = texture_x % sky_width; // Wrap texture_x
			pixel = &(texture_sky->pixels[(texture_y * sky_width + texture_x)
					* texture_sky->bytes_per_pixel]);
			mlx_put_pixel(global->img, x, y, get_rgba(pixel[0], pixel[1],
					pixel[2], 255));
			x++;
		}
		y++;
	}
	while (y < global->window_height)
	{
		texture_y = (int)(((float)(y - global->window_height / 2)
					/ (global->window_height / 2)) * texture_floor->height);
		x = 0;
		while (x < global->window_width)
		{
			texture_x = (int)(((float)(x + floor_offset) / global->window_width)
					* floor_width);
			texture_x = texture_x % floor_width; // Wrap texture_x
			pixel = &(texture_floor->pixels[(texture_y * floor_width
						+ texture_x) * texture_floor->bytes_per_pixel]);
			mlx_put_pixel(global->img, x, y, get_rgba(pixel[0], pixel[1],
					pixel[2], 255));
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

int	get_wall_color(t_face face)
{
	if (face == NORTH)
		return (NORTH_COLOR);
	if (face == SOUTH)
		return (SOUTH_COLOR);
	if (face == EAST)
		return (EAST_COLOR);
	if (face == WEST)
		return (WEST_COLOR);
	return (get_rgba(255, 255, 255, 255));
	// Default color if something goes wrong
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
	// Check if textures are loaded successfully
	if (!texture_north || !texture_south || !texture_east || !texture_west)
	{
		fprintf(stderr, "Error loading textures\n");
		return ;
	}
	player_angle = atan2(global->player->dir.y, global->player->dir.x);
	bar_width = 1;
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
