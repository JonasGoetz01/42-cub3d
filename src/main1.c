/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:26:41 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 11:46:34 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	init_to_null(t_global *global)
{
	global->map = NULL;
	global->player = NULL;
	global->doors = NULL;
	global->door_line = NULL;
	global->texture = NULL;
	global->lines = NULL;
}

void	ft_exit_free(t_global *global)
{
	if (global->map != NULL)
		ft_arr_free((void **)global->map);
	if (global->player != NULL)
	{
		if (global->player->rays != NULL)
			free(global->player->rays);
		free(global->player);
	}
	if (global->doors != NULL)
		free(global->doors);
	if (global->door_line != NULL)
		free(global->door_line);
	if (global->texture != NULL)
		free(global->texture);
	if (global->lines != NULL)
		free(global->lines);
	exit(EXIT_FAILURE);
}
