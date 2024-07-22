/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_size.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 12:42:05 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 17:35:14 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	process_map_line(t_map_size *var)
{
	if (var->line[0] == '\n')
	{
		var->empty_line = 1;
		free_and_null((void **)&(var->line));
		return (0);
	}
	if (var->empty_line && var->line[0] != '\n')
	{
		free_and_null((void **)&(var->line));
		return (printf(ERR_EMPTY), 1);
	}
	var->tmp = ft_strtrim(var->line, "\n");
	free_and_null((void **)&(var->line));
	var->height++;
	if (strlen_tab_to_space(var->tmp) > var->max_width)
		var->max_width = strlen_tab_to_space(var->tmp);
	var->in_map = 1;
	var->empty_line = 0;
	free(var->tmp);
	return (0);
}

int	process_file(t_map_size *var, t_global *global)
{
	while (1)
	{
		var->line = get_next_line(var->fd);
		if (!var->line)
			break ;
		if (var->line[0] == '\n' && !var->in_map)
		{
			free_and_null((void **)&(var->line));
			continue ;
		}
		if (var->i < 6)
		{
			if (!check_identifier(var->line, global))
				return (free_and_null((void **)&(var->line)),
					free(var->tmp), 1);
			var->i++;
		}
		else
		{
			if (process_map_line(var))
				return (1);
		}
		free_and_null((void **)&(var->line));
	}
	return (0);
}

void	init_vars(t_map_size *var)
{
	var->line = NULL;
	var->tmp = NULL;
	var->height = 0;
	var->max_width = 0;
	var->in_map = 0;
	var->i = 0;
	var->empty_line = 0;
}

int	map_size(char *file, t_global *global)
{
	t_map_size	var;

	init_vars(&var);
	var.fd = open(file, O_RDONLY);
	if (var.fd < 0)
		return (printf(ERR_MALLOC), 1);
	if (process_file(&var, global))
		return (close(var.fd), 1);
	global->map->height = var.height;
	global->map->width = var.max_width;
	return (0);
}
