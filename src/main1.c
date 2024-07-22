/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:26:41 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 18:42:33 by jgotz            ###   ########.fr       */
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

void	free_map(t_global *global)
{
	int	i;

	if (global->map != NULL)
	{
		if (global->map->map != NULL)
		{
			i = 0;
			while (i < global->map->height)
			{
				if (global->map->map[i] != NULL)
				{
					free(global->map->map[i]);
				}
				i++;
			}
			free(global->map->map);
		}
		free(global->map);
	}
}


void	free_player_rays(t_global *global)
{
	int	i;

	if (global->player != NULL && global->player->rays != NULL)
	{
		i = 0;
		while (i < WIDTH)
			free(&(global->player->rays[i++]));
	}
}

void	free_textures(t_global *global)
{
	if (global->texture != NULL)
	{
		if (global->texture->west != NULL)
			free(global->texture->west);
		if (global->texture->east != NULL)
			free(global->texture->east);
		if (global->texture->north != NULL)
			free(global->texture->north);
		if (global->texture->south != NULL)
			free(global->texture->south);
		free(global->texture);
	}
}

void	ft_exit_free(t_global *global)
{
	free_map(global);
	free_player_rays(global);
	if (global->player != NULL)
		free(global->player);
	if (global->doors != NULL)
		free(global->doors);
	if (global->door_line != NULL)
		free(global->door_line);
	free_textures(global);
	if (global->lines != NULL)
		free(global->lines);
	exit(EXIT_FAILURE);
}
