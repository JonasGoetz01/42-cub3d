#include "../../inc/cub3d.h"

t_vec2d	get_player_position(t_global *global)
{
	t_vec2d	player_pos;

	player_pos = (t_vec2d){0, 0};
	for (int y = 0; y < global->map->height; y++)
	{
		for (int x = 0; x < global->map->width; x++)
		{
			if (global->map->map[y][x] == 'N' || global->map->map[y][x] == 'E'
				|| global->map->map[y][x] == 'W'
				|| global->map->map[y][x] == 'S')
			{
				player_pos.x = x;
				player_pos.y = y;
				return (player_pos);
			}
		}
	}
	return (player_pos);
}

t_vec2d	get_player_direction(t_global *global)
{
	t_vec2d	player_pos;

	player_pos = (t_vec2d){0, 0};
	for (int y = 0; y < global->map->height; y++)
	{
		for (int x = 0; x < global->map->width; x++)
		{
			if (global->map->map[y][x] == 'N')
				return ((t_vec2d){0, -1});
			else if (global->map->map[y][x] == 'E')
				return ((t_vec2d){1, 0});
			else if (global->map->map[y][x] == 'W')
				return ((t_vec2d){-1, 0});
			else if (global->map->map[y][x] == 'S')
				return ((t_vec2d){0, 1});
		}
	}
	return (player_pos);
}

void	get_doors(t_global *global)
{
	int count;

	count = 0;
	for (int y = 0; y < global->map->height; y++)
	{
		for (int x = 0; x < global->map->width; x++)
		{
			if (global->map->map[y][x] == 'D')
				count++;
		}
	}
	global->door_count = count;
	global->doors = malloc(sizeof(t_door) * count);
	count = 0;
	for (int y = 0; y < global->map->height; y++)
	{
		for (int x = 0; x < global->map->width; x++)
		{
			if (global->map->map[y][x] == 'D')
			{
				global->doors[count].pos.x = x * global->scale_factor;
				global->doors[count].pos.y = y * global->scale_factor;
				global->doors[count].state = CLOSED;
				count++;
			}
		}
	}
}
