/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:27:52 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/09 19:45:03 by cgerling         ###   ########.fr       */
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
		printf("color: %s\n", color[i[0]]);
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
	(void)global;
	printf("map: %s\n", line);
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
	fd = open(file, O_RDONLY); // maybe make it to have fd here already because of valid_file then i wouldn't need to open it again
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
		if (i < 6) // probably need to modify conditions to determine line or map
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

void map_size(char *file, t_global *global)
{
	char *line;
	int fd;
	int height;
	int max_width;
	int in_map;

	in_map = 0;
	height = 0;
	max_width = 0;
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		// error
		return ;
	}
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] == '\n' && !in_map)
			continue ;
		if (check_for_map(line)) // need to implement this function
			in_map = 1;
		if (in_map)
		{
			if (line[0] == '\n')
			{
				// error
				return ;
			}
			height++;
			if (ft_strlen(line) > max_width)
				max_width = ft_strlen(line);
		}
		free(line);
	}
	global->map->height = height;
	global->map->width = max_width;
}

int parse_and_validate(char *file, t_global *global)
{
	if (!valid_file(file, 0))
	{
		printf("\noption1\n");
		return (1);
	}
	global->texture = malloc(sizeof(t_texture));
	if (!global->texture)
	{
		// error
		return (1);
	}
	// global->map->map = malloc();
	if (parse_file(file, global))
	{
		printf("\noption2\n");
		return (1);
	}
	printf("\noption3\n");
	return (0);
}
