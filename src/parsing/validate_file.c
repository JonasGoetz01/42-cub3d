/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 16:20:59 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/10 15:52:50 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

bool	check_extension(char *file, int flag)
{
	if (flag)
	{
		if (ft_strncmp(file + ft_strlen(file) - 4, ".png", 4))
		{
			printf("Error: Invalid texture file extension\n");
			return (false);
		}
	}
	else
	{
		if (ft_strncmp(file + ft_strlen(file) - 4, ".cub", 4))
		{
			printf("Error: Invalid map file extension\n");
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
		printf("Error: File is a directory\n");
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
		printf("Error: Could not open file\n");
		return (false);
	}
	close(fd);
	return (true);
}
