/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:26:41 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 11:46:10 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	init_global(t_global *global)
{
	init_to_null(global);
	global->window_height = HEIGHT;
	global->window_width = WIDTH;
	global->minimap_scale = MINIMAP_SCALE;
	global->time = get_current_millis();
	global->open = false;
	global->close = false;
	global->free_mouse = false;
	global->sprite_index = 0;
}

void	load_textures(t_global *global)
{
	global->sprite_textures[0] = mlx_load_png("textures/gun1.png");
	global->sprite_textures[1] = mlx_load_png("textures/gun2.png");
	global->sprite_textures[2] = mlx_load_png("textures/gun3.png");
	global->sprite_textures[3] = mlx_load_png("textures/gun2.png");
}

void	init_mlx(t_global *global)
{
	if (!(global->mlx = mlx_init(global->window_width, global->window_height,
				"cub3d", false)))
		ft_exit_free(global);
	global->minimap = mlx_new_image(global->mlx, global->window_width,
			global->window_height);
	global->img = mlx_new_image(global->mlx, global->window_width,
			global->window_height);
	if (!global->img || !global->minimap)
		ft_exit_free(global);
}

void	setup_hooks(t_global *global)
{
	mlx_loop_hook(global->mlx, key_hook_movement, global);
	global->player = new_player(global, (t_vec2d){get_player_position(global).x
			* global->scale_factor, get_player_position(global).y
			* global->scale_factor}, get_player_direction(global));
	mlx_loop_hook(global->mlx, loop, global);
	mlx_resize_hook(global->mlx, resize, global);
	mlx_cursor_hook(global->mlx, cursor, global);
	mlx_key_hook(global->mlx, key_hook, global);
}

int	main(int argc, char **argv)
{
	t_global	global;

	if (argc != 2)
		return (printf(ERR_ARG USAGE), EXIT_FAILURE);
	init_global(&global);
	if (parse_and_validate(argv[1], &global))
		ft_exit_free(&global);
	init_mlx(&global);
	load_textures(&global);
	setup_hooks(&global);
	mlx_loop(global.mlx);
	mlx_terminate(global.mlx);
	return (0);
}
