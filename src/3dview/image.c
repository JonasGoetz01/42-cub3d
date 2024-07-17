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
	return (height);
}

float	get_distance(t_vec2d a, t_vec2d b)
{
	return (sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2)));
}

void	load_textures(mlx_texture_t **texture_north,
		mlx_texture_t **texture_south, mlx_texture_t **texture_east,
		mlx_texture_t **texture_west)
{
	if (!*texture_north)
		*texture_north = mlx_load_png("textures/cobblestone.png");
	if (!*texture_south)
		*texture_south = mlx_load_png("textures/dirt.png");
	if (!*texture_east)
		*texture_east = mlx_load_png("textures/polished_granite.png");
	if (!*texture_west)
		*texture_west = mlx_load_png("textures/piston_bottom.png");
	if (!*texture_north || !*texture_south || !*texture_east || !*texture_west)
	{
		fprintf(stderr, "Error loading textures\n");
	}
}

mlx_texture_t	*select_texture(t_collision *collision,
		mlx_texture_t *texture_north, mlx_texture_t *texture_south,
		mlx_texture_t *texture_east, mlx_texture_t *texture_west)
{
	if (collision->face == NORTH)
		return (texture_north);
	else if (collision->face == SOUTH)
		return (texture_south);
	else if (collision->face == EAST)
		return (texture_east);
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

	int r, g, b, color;
	int draw_y, texture_x, texture_y, i;
	texture_x = (int)(hit_percentage * (texture->width));
	int i = 0;
	while (i < bar_height)
	{
		texture_y = (int)(((float)i / bar_height) * (texture->height));
		texture_y = fmax(fmin(texture_y, texture->height - 1), 0);
		texture_x = fmax(fmin(texture_x, texture->width - 1), 0);
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

void	render_3d(t_global *global)
{
	static mlx_texture_t	*texture_north = NULL;
	static mlx_texture_t	*texture_south = NULL;
	static mlx_texture_t	*texture_east = NULL;
	static mlx_texture_t	*texture_west = NULL;
	t_ray					*ray;
	t_collision				*collision;
	mlx_texture_t			*texture;
	int						i;

	float player_angle, ray_angle, angle_diff;
	float distance, perpendicular_distance, hit_percentage;
	int bar_width, bar_height, center_y, top_y, bottom_y, x;
	load_textures(&texture_north, &texture_south, &texture_east, &texture_west);
	player_angle = atan2(global->player->dir.y, global->player->dir.x);
	bar_width = 1;
	i = 0;
	while (i < (int)global->img->width)
	{
		ray = &global->player->rays[i];
		collision = ray->closest_collision;
		if (collision)
		{
			distance = get_distance(global->player->pos, collision->point);
			distance = fmax(distance, 0.1f);
			ray_angle = atan2(ray->direction.y, ray->direction.x);
			angle_diff = ray_angle - player_angle;
			perpendicular_distance = distance * cos(angle_diff);
			bar_height = map_distance_to_height(perpendicular_distance, global);
			center_y = global->img->height / 2;
			top_y = center_y - (bar_height / 2);
			bottom_y = center_y + (bar_height / 2);
			x = i * bar_width;
			texture = select_texture(collision, texture_north, texture_south,
					texture_east, texture_west);
			hit_percentage = calculate_hit_percentage(collision);
			hit_percentage = fmax(fmin(hit_percentage, 1.0f), 0.0f);
			draw_column(global, x, top_y, bar_height, texture, hit_percentage);
		}
		i++;
	}
}
