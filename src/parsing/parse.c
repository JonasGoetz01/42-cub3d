/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:27:52 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/18 17:42:01 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

// clean exit function that frees all mallocs

int	parse_configurations(int fd, t_global *global)
{
	char	*line;
	char	*tmp;
	int		i;

	i = 0;
	while (i < 6)
	{
		line = get_next_line(fd);
		if (!line)
			return (1);
		if (line[0] == '\n')
		{
			free(line);
			continue ;
		}
		tmp = ft_strtrim(line, "\n");
		if (!tmp)
			return (printf(ERR_MALLOC), free(line), 1);
		free(line);
		if (!parse_line(tmp, global))
			return (free(tmp), 1);
		free(tmp);
		i++;
	}
	return (0);
}

int	parse_map_data(int fd, t_global *global, int count)
{
	char	*line;
	char	*tmp;

	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] == '\n')
		{
			free(line);
			continue ;
		}
		tmp = ft_strtrim(line, "\n");
		if (!tmp)
			return (printf(ERR_MALLOC), free(line), 1);
		free(line);
		if (!parse_map(tmp, global))
			return (free(tmp), 1);
		free(tmp);
		count++;
		if (count == global->map->height)
			break ;
	}
	return (0);
}

int	parse_file(char *file, t_global *global)
{
	int	fd;
	int	ret;
	int	count;

	count = 0;
	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (printf(ERR_OPEN), 1);
	ret = parse_configurations(fd, global);
	if (ret)
		return (close(fd), 1);
	ret = parse_map_data(fd, global, count);
	close(fd);
	return (ret);
}

void	configure_map(t_global *global)
{
	t_line	*lines;
	int		line_count;
	int		count;
	int		i;

	count = 0;
	i = -1;
	global->scale_factor = calculate_scale_factor(global->map->width,
			global->map->height, WIDTH * global->minimap_scale, HEIGHT
			* global->minimap_scale);
	get_doors(global);
	global->door_line = malloc(sizeof(t_line *) * (global->door_count + 1));
	map_to_line_segments(global, &lines, &line_count);
	scale_line_segments(lines, line_count, global->scale_factor);
	global->line_count = line_count;
	global->lines = lines;
	while (++i < global->line_count)
	{
		if (global->lines[i].type == DOOR)
			global->door_line[count++] = &global->lines[i];
	}
	global->door_line[count] = NULL;
	get_opponents(global);
}

int	parse_and_validate(char *file, t_global *global)
{
	if (!valid_file(file, 0))
		return (1);
	global->texture = malloc(sizeof(t_texture));
	global->map = malloc(sizeof(t_map));
	if (!global->texture || !global->map)
		return (printf(ERR_MALLOC), 1);
	global->flags = (t_identifier_flag){0, 0, 0, 0, 0, 0};
	if (map_size(file, global))
		return (1);
	global->map->count = 0;
	global->map->map = malloc(sizeof(char *) * (global->map->height + 1));
	if (!global->map->map)
		return (printf(ERR_MALLOC), 1);
	if (parse_file(file, global))
		return (1);
	if (!valid_map(global->map->map, global->map->height))
		return (1);
	configure_map(global);
	return (0);
}
