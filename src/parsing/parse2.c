/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 12:43:32 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/11 12:54:59 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

bool	parse_texture(char *identifier, char *path,
		t_global *global, char **to_free)
{
	if (!valid_file(path, 1))
		return (ft_arr_free((void **)to_free), false);
	if (ft_strcmp(identifier, "NO") == 0)
		global->texture->north = ft_strdup(path);
	else if (ft_strcmp(identifier, "SO") == 0)
		global->texture->south = ft_strdup(path);
	else if (ft_strcmp(identifier, "WE") == 0)
		global->texture->west = ft_strdup(path);
	else if (ft_strcmp(identifier, "EA") == 0)
		global->texture->east = ft_strdup(path);
	ft_arr_free((void **) to_free);
	return (true);
}

bool	parse_color_long(char **split, t_global *global)
{
	char	**colors;
	int		tmp[3];

	colors = ft_split(split[1], ',');
	if (!colors || !check_arg_amount(colors, 3))
		return (printf(ERR), ft_arr_free((void **)split),
			ft_arr_free((void **)colors), false);
	tmp[0] = ft_atoi(colors[0]);
	tmp[1] = ft_atoi(colors[1]);
	tmp[2] = ft_atoi(colors[2]);
	if (!valid_range(tmp) || !strlen_check(colors))
		return (ft_arr_free((void **)split), ft_arr_free((void **)colors), 0);
	if (ft_strcmp(split[0], "F") == 0)
	{
		global->floor.r = tmp[0];
		global->floor.g = tmp[1];
		global->floor.b = tmp[2];
	}
	else if (ft_strcmp(split[0], "C") == 0)
	{
		global->ceiling.r = tmp[0];
		global->ceiling.g = tmp[1];
		global->ceiling.b = tmp[2];
	}
	return (ft_arr_free((void **)split), ft_arr_free((void **)colors), true);
}

bool	parse_color_short(char **split, t_global *global)
{
	int	tmp[3];

	tmp[0] = ft_atoi(split[1]);
	tmp[1] = ft_atoi(split[2]);
	tmp[2] = ft_atoi(split[3]);
	if (ft_strlen(split[1]) > 11 || ft_strlen(split[2]) > 11
		|| ft_strlen(split[3]) > 11)
		return (printf(ERR_RANGE), ft_arr_free((void **)split), false);
	if (!valid_range(tmp))
		return (ft_arr_free((void **)split), false);
	if (ft_strcmp(split[0], "F") == 0)
	{
		global->floor.r = tmp[0];
		global->floor.g = tmp[1];
		global->floor.b = tmp[2];
	}
	else if (ft_strcmp(split[0], "C") == 0)
	{
		global->ceiling.r = tmp[0];
		global->ceiling.g = tmp[1];
		global->ceiling.b = tmp[2];
	}
	return (ft_arr_free((void **)split), true);
}

bool	parse_line(char *line, t_global *global)
{
	char	**split;

	split = ft_split(line, ' ');
	if (!split)
		return (printf(ERR_MALLOC), false);
	if (check_arg_amount(split, 2) && (ft_strcmp(split[0], "NO") == 0
			|| ft_strcmp(split[0], "SO") == 0 || ft_strcmp(split[0], "WE") == 0
			|| ft_strcmp(split[0], "EA") == 0))
		return (parse_texture(split[0], split[1], global, split));
	else if ((ft_strcmp(split[0], "F") == 0 || ft_strcmp(split[0], "C") == 0)
		&& (check_arg_amount(split, 2) || check_arg_amount(split, 4)))
	{
		if (!color_format(split))
			return (ft_arr_free((void **)split), false);
		if (check_arg_amount(split, 2))
			return (parse_color_long(split, global));
		else
			return (parse_color_short(split, global));
	}
	else
		return (printf(ERR_NO_ID), ft_arr_free((void **)split), false);
}

bool	parse_map(char *line, t_global *global)
{
	global->map->map[global->map->count]
		= strdup_tab_to_space(line, global->map->width);
	if (!global->map->map[global->map->count])
		return (printf(ERR_MALLOC), false);
	global->map->count++;
	if (global->map->count == global->map->height)
	{
		global->map->map[global->map->count] = NULL;
	}
	return (true);
}
