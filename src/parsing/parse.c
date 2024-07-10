/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:27:52 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/10 11:27:14 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

// step 1: validate input file
// step 2: read input file to get map size
// step 3: read input file
// step 4: validate texture paths and colors
// step 5: parse textures and colors
// step 6: validate map
// step 7: parse map

// clean exit function that frees all mallocs
// error function that prints certain error message
// error messages will be modified later on to be more specific

int	ft_strcmp(const char *s1, const char *s2) // add to libft?
{
	char	*str1;
	char	*str2;
	int		i;

	i = 0;
	str1 = (char *)s1;
	str2 = (char *)s2;
	while (str1[i] && str2[i] && str1[i] == str2[i])
		i++;
	return (str1[i] - str2[i]);
}

int strlen_tab_to_space(char *str)
{
	int i;

	i = 0;
	while (*str)
	{
		if (*str == '\t')
			i += 4;
		else
			i++;
		str++;
	}
	return (i);
}

char	*strdup_tab_to_space(const char *s1, int len)
{
	int		i;
	char	*str;

	i = 0;
	str = malloc(len + 1);
	if (str == NULL)
		return (NULL);
	while (*s1)
	{
		if (*s1 == '\t')
		{
			str[i++] = ' ';
			str[i++] = ' ';
			str[i++] = ' ';
			str[i++] = ' ';
		}
		else
			str[i++] = *s1;
		s1++;
	}
	while (i < len)
		str[i++] = ' ';
	str[i] = '\0';
	return (str);
}


bool check_arg_amount(char **split, int amount)
{
	int i;

	i = 0;
	while (split[i])
		i++;
	if (i != amount)
		return (false);
	return (true);
}

bool valid_range(int color)
{
	if (color < 0 || color > 255)
	{
		// error
		return (false);
	}
	return (true);
}

bool parse_texture(char *identifier, char *path, t_global *global)
{
	if (!valid_file(path, 1))
		return (false);
	if (ft_strcmp(identifier, "NO") == 0)
		global->texture->north = ft_strdup(path);
	else if (ft_strcmp(identifier, "SO") == 0)
		global->texture->south = ft_strdup(path);
	else if (ft_strcmp(identifier, "WE") == 0)
		global->texture->west = ft_strdup(path);
	else if (ft_strcmp(identifier, "EA") == 0)
		global->texture->east = ft_strdup(path);
	return (true);
}

bool parse_color_long(char **split, t_global *global)
{
	char	**colors;
	int		tmp[3];

	colors = ft_split(split[1], ',');
	if (!colors || !check_arg_amount(colors, 3))
	{
		// error
		return (false);
	}
	tmp[0] = ft_atoi(colors[0]);
	tmp[1] = ft_atoi(colors[1]);
	tmp[2] = ft_atoi(colors[2]);
	if (!valid_range(tmp[0]) || !valid_range(tmp[1]) || !valid_range(tmp[2]))
	{
		// error
		return (false);
	}
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
	return (true);
}

bool parse_color_short(char **split, t_global *global)
{
	int tmp[3];

	tmp[0] = ft_atoi(split[1]);
	tmp[1] = ft_atoi(split[2]);
	tmp[2] = ft_atoi(split[3]);
	if (!valid_range(tmp[0]) || !valid_range(tmp[1]) || !valid_range(tmp[2]))
	{
		// error
		return (false);
	}
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
	return (true);
}

bool precise_comma_check(char **color)
{
	int i;
	int j;

	i = 0;
	if (check_arg_amount(color, 4))
	{
		// comma check for three seperate arguments for r, g, b
		while (color[++i])
		{
			j = ft_strlen(color[i]);
			if ((i < 3 && color[i][j - 1] != ',') || (i == 3 && ft_strchr(color[i], ',')) || j > 4)
			{
				// error
				printf("error comma multiple\n");
				return (false);
			}
		}
	}
	else
	{
		// comma check for one argument for r, g, b
		while (color[++i])
		{
			j = 0;
			while (color[i][j])
			{
				if (color[i][0] == ',' || (color[i][j] == ',' && (color[i][j + 1] == ',' || color[i][j + 1] == '\0')) || ft_strlen(color[i]) > 11)
				{
					// error
					printf("error comma single\n");
					return (false);
				}
				j++;
			}
		}
	}
	return (true);
}

bool color_format(char **color)
{
	int i[2];
	int comma;

	i[0] = 1;
	comma = 0;
	while (color[i[0]])
	{
		i[1] = 0;
		while (color[i[0]][i[1]])
		{
			if (color[i[0]][i[1]] == ',')
				comma++;
			if ((!ft_isdigit(color[i[0]][i[1]]) && color[i[0]][i[1]] != ',') || comma > 2)
			{
				// error
				printf("error color format\n");
				return (false);
			}
			i[1]++;
		}
		i[0]++;
	}
	return (precise_comma_check(color));
}

bool parse_line(char *line, t_global *global)
{
	char	**split;

	split = ft_split(line, ' ');
	if (!split)
	{
		// error
		return (false);
	}
	if (check_arg_amount(split, 2) && (ft_strcmp(split[0], "NO") == 0 || ft_strcmp(split[0], "SO") == 0 || ft_strcmp(split[0], "WE") == 0 || ft_strcmp(split[0], "EA") == 0))
		return (parse_texture(split[0], split[1], global));
	else if ((ft_strcmp(split[0], "F") == 0 || ft_strcmp(split[0], "C") == 0) && (check_arg_amount(split, 2) || check_arg_amount(split, 4)))
	{
		// comma check meaning not more than one comma in between values, no idea how implement this the best way yet
		if (!color_format(split))
		{
			// error
			return (false);
		}
		if (check_arg_amount(split, 2))
			return (parse_color_long(split, global));
		else if (check_arg_amount(split, 4))
			return (parse_color_short(split, global));
		else
		{
			printf("error color\n");
			// error
			return (false);
		}
	}
	else
	{
		printf("error identifier\n");
		// error invalid identifier
		return (false);
	}
}

bool parse_map(char *line, t_global *global)
{
	global->map->map[global->map->count] = strdup_tab_to_space(line, global->map->width);
	global->map->count++;
	if (global->map->count == global->map->height)
	{
		global->map->map[global->map->count] = NULL;
	}
	return (true);
}

int parse_file(char *file, t_global *global)
{
	int fd;
	int i;
	char *line;
	char *tmp;
	int in_map;
	
	i = 0;
	in_map = 0;
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		// error
		return (1);
	}
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] == '\n' && !in_map)
			continue ;
		tmp = ft_strtrim(line, "\n");
		free(line);
		if (i < 6)
		{
			if (!parse_line(tmp, global))
			{
				// error
				return (1);
			}
			else
				i++;
		}
		else
		{
			if (!parse_map(tmp, global))
			{
				// error
				return (1);
			}
			else
				in_map = 1;
		}
		free(tmp);
	}
	return (0);
}

int map_size(char *file, t_global *global)
{
	char *line;
	char *tmp;
	int fd;
	int height;
	int max_width;
	int in_map;
	int i;

	in_map = 0;
	height = 0;
	max_width = 0;
	i = 0;
	tmp = NULL;
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		// error
		return (1);
	}
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] == '\n' && !in_map)
			continue ;
		if (i < 6)
		{
			i++;
		}
		else
		{
			in_map = 1;
			if (line[0] == '\n')
			{
				printf("error map size\n");
				// error
				return (1);
			}
			tmp = ft_strtrim(line, "\n");
			free(line);
			height++;
			if (strlen_tab_to_space(tmp) > max_width)
				max_width = strlen_tab_to_space(tmp);
		}
		free(tmp);
	}
	global->map->height = height;
	global->map->width = max_width;
	return (0);
}

int parse_and_validate(char *file, t_global *global)
{
	t_line *lines;
    int line_count;
	
	if (!valid_file(file, 0))
		return (1);
	global->texture = malloc(sizeof(t_texture));
	if (!global->texture)
	{
		// error
		return (1);
	}
	global->map = malloc(sizeof(t_map));
	if (map_size(file, global))
		return (1);
	global->map->count = 0;
	global->map->map = malloc(sizeof(char *) * (global->map->height + 1));
	if (!global->map->map)
	{
		// error
		return (1);
	}
	if (parse_file(file, global))
		return (1);
	map_to_line_segments(global, &lines, &line_count);
    global->scale_factor = calculate_scale_factor(global->map->width, global->map->height, WIDTH * global->minimap_scale, HEIGHT * global->minimap_scale);
    scale_line_segments(lines, line_count, global->scale_factor);
    global->line_count = line_count;
    global->lines = lines;
    get_opponents(global);
	return (0);
}
