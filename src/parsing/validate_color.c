/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_color.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 11:06:21 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/11 12:58:32 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

bool	strlen_check(char **split)
{
	int	i;

	i = 0;
	while (split[++i])
	{
		if (ft_strlen(split[i]) > 11)
			return (printf(ERR_RANGE), false);
	}
	return (true);
}

bool	precise_comma_check2(char **color, int comma)
{
	int	i;
	int	j;

	i = 0;
	while (color[++i])
	{
		j = 0;
		while (color[i][j])
		{
			if (color[i][0] == ',' || (color[i][j] == ','
				&& (color[i][j + 1] == ',' || color[i][j + 1] == '\0'))
				|| comma > 2)
				return (printf(ERR_COMMA), false);
			j++;
		}
	}
	return (true);
}

bool	precise_comma_check(char **color, int comma)
{
	int	i;
	int	j;

	i = 0;
	if (check_arg_amount(color, 4))
	{
		while (color[++i])
		{
			j = ft_strlen(color[i]);
			if ((i < 3 && color[i][j - 1] != ',')
				|| (i == 3 && ft_strchr(color[i], ',')) || comma > 2)
				return (printf(ERR_COMMA), false);
		}
	}
	else
		return (precise_comma_check2(color, comma));
	return (true);
}

bool	color_format(char **color)
{
	int	i[2];
	int	comma;

	i[0] = 1;
	comma = 0;
	while (color[i[0]])
	{
		i[1] = 0;
		while (color[i[0]][i[1]])
		{
			if (color[i[0]][i[1]] == ',')
				comma++;
			if ((!ft_isdigit(color[i[0]][i[1]]) && color[i[0]][i[1]] != ','))
			{
				printf(RED BOLD "ERROR: Invalid characters in color\n" NC);
				return (false);
			}
			i[1]++;
		}
		i[0]++;
	}
	return (precise_comma_check(color, comma));
}
