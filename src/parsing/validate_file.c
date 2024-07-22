/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgotz <jgotz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 16:20:59 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/22 17:33:41 by jgotz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

bool	check_extension(char *file, int flag)
{
	if (flag)
	{
		if (ft_strncmp(file + ft_strlen(file) - 4, ".png", 4))
		{
			printf(RED BOLD "ERROR: Invalid texture file extension\n" NC);
			return (false);
		}
	}
	else
	{
		if (ft_strncmp(file + ft_strlen(file) - 4, ".cub", 4))
		{
			printf(RED BOLD "ERROR: Invalid map file extension\n" NC);
			return (false);
		}
	}
	return (true);
}

bool	check_dir(char *file)
{
	int	fd;

	fd = open(file, O_DIRECTORY);
	if (fd >= 0)
	{
		printf(ERR_DIR);
		close(fd);
		return (true);
	}
	return (false);
}

bool	valid_file(char *file, int flag)
{
	int	fd;

	if (check_dir(file))
		return (false);
	if (!check_extension(file, flag))
		return (false);
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		printf(ERR_OPEN "%s\n" NC, file);
		return (false);
	}
	close(fd);
	return (true);
}
