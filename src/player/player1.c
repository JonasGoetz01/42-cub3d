/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:26:41 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 13:55:58 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	update_sprite_index(t_global *global, t_vec2d dir)
{
	static int	sprite_counter = 0;

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
}

bool	check_collision(t_vec2d pos, float radius, t_global *global)
{
	int	i;

	i = 0;
	while (i < global->line_count)
	{
		if (circle_line_collision(pos, radius, global->lines[i]))
			return (true);
		i++;
	}
	return (false);
}

void	update_position_component(t_global *global, t_vec2d new_pos,
		bool *collision, char axis)
{
	t_vec2d	temp_pos;

	temp_pos = global->player->pos;
	if (axis == 'x')
		temp_pos.x = new_pos.x;
	else if (axis == 'y')
		temp_pos.y = new_pos.y;
	*collision = check_collision(temp_pos, PLAYER_RADIUS * global->scale_factor,
			global);
	if (!*collision)
	{
		if (axis == 'x')
			global->player->pos.x = new_pos.x;
		else if (axis == 'y')
			global->player->pos.y = new_pos.y;
	}
}

void	update_rays_origin(t_global *global)
{
	int	i;

	i = 0;
	while (i < (int)global->img->width)
	{
		global->player->rays[i].origin = global->player->pos;
		i++;
	}
}

void	update_position(t_global *global, t_vec2d dir, float speed)
{
	t_vec2d	new_pos;
	bool	collision_x;
	bool	collision_y;

	update_sprite_index(global, dir);
	new_pos.x = global->player->pos.x + dir.x * speed * global->minimap_scale;
	new_pos.y = global->player->pos.y + dir.y * speed * global->minimap_scale;
	update_position_component(global, new_pos, &collision_x, 'x');
	update_position_component(global, new_pos, &collision_y, 'y');
	update_rays_origin(global);
}
