/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:26:41 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 11:44:37 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_global	*global;

	if (keydata.key == MLX_KEY_LEFT_ALT && keydata.action == MLX_PRESS)
	{
		global = (t_global *)param;
		global->free_mouse = !global->free_mouse;
	}
	if (keydata.key == MLX_KEY_LEFT_CONTROL && keydata.action == MLX_PRESS)
		global->open = true;
	if (keydata.key == MLX_KEY_LEFT_CONTROL && keydata.action == MLX_RELEASE)
		global->open = false;
	if (keydata.key == MLX_KEY_RIGHT_CONTROL && keydata.action == MLX_PRESS)
		global->close = true;
	if (keydata.key == MLX_KEY_RIGHT_CONTROL && keydata.action == MLX_RELEASE)
		global->close = false;
}

void	key_hook_movement(void *param)
{
	t_global	*global;
	t_vec2d		dir;
	float		move_speed;
	float		length;

	dir = (t_vec2d){0.0f, 0.0f};
	global = (t_global *)param;
	move_speed = MOVE_SPEED * global->scale_factor / 8;
	if (mlx_is_key_down(global->mlx, MLX_KEY_ESCAPE))
		return (mlx_close_window(global->mlx));
	if (mlx_is_key_down(global->mlx, MLX_KEY_LEFT_SHIFT))
		move_speed *= 2;
	if (mlx_is_key_down(global->mlx, MLX_KEY_W))
	{
		dir.x += global->player->dir.x;
		dir.y += global->player->dir.y;
	}
	if (mlx_is_key_down(global->mlx, MLX_KEY_S))
	{
		dir.x -= global->player->dir.x;
		dir.y -= global->player->dir.y;
	}
	if (mlx_is_key_down(global->mlx, MLX_KEY_A))
	{
		dir.x += global->player->dir.y;
		dir.y -= global->player->dir.x;
	}
	if (mlx_is_key_down(global->mlx, MLX_KEY_D))
	{
		dir.x -= global->player->dir.y;
		dir.y += global->player->dir.x;
	}
	if (mlx_is_key_down(global->mlx, MLX_KEY_E))
		rotate_player(global, 0.03f);
	if (mlx_is_key_down(global->mlx, MLX_KEY_Q))
		rotate_player(global, -0.03f);
	length = sqrtf(dir.x * dir.x + dir.y * dir.y);
	if (length != 0)
	{
		dir.x /= length;
		dir.y /= length;
	}
	update_position(global, dir, move_speed);
}
