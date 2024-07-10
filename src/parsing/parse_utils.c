/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 15:17:15 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/10 18:27:12 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	ft_strcmp(const char *s1, const char *s2)
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

int	strlen_tab_to_space(char *str)
{
	int	i;

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

bool	check_arg_amount(char **split, int amount)
{
	int	i;

	i = 0;
	while (split[i])
		i++;
	if (i != amount)
		return (false);
	return (true);
}

bool	valid_range(int color)
{
	if (color < 0 || color > 255)
	{
		printf(ERR_RANGE);
		return (false);
	}
	return (true);
}
