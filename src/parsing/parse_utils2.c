/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 11:04:18 by cgerling          #+#    #+#             */
/*   Updated: 2024/07/11 12:52:45 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

bool	set_flag(bool *flag)
{
	*flag = 1;
	return (true);
}

bool	double_identifier(char *identifier, t_global *global)
{
	if (ft_strcmp(identifier, "NO") == 0 && global->flags.no)
		return (true);
	else if (ft_strcmp(identifier, "SO") == 0 && global->flags.so)
		return (true);
	else if (ft_strcmp(identifier, "WE") == 0 && global->flags.we)
		return (true);
	else if (ft_strcmp(identifier, "EA") == 0 && global->flags.ea)
		return (true);
	else if (ft_strcmp(identifier, "F") == 0 && global->flags.f)
		return (true);
	else if (ft_strcmp(identifier, "C") == 0 && global->flags.c)
		return (true);
	return (false);
}

bool	check_identifier(char *line, t_global *global)
{
	char	**split;

	split = ft_split(line, ' ');
	if (!split)
		return (printf(RED BOLD ERR_MALLOC NC), false);
	if (double_identifier(split[0], global))
		return (ft_arr_free((void **)split), printf(ERR_D_ID), false);
	if (!(check_arg_amount(split, 2) || check_arg_amount(split, 4)))
		return (ft_arr_free((void **)split), printf(ERR), false);
	if (ft_strcmp(split[0], "NO") == 0)
		return (ft_arr_free((void **)split), set_flag(&global->flags.no));
	else if (ft_strcmp(split[0], "SO") == 0)
		return (ft_arr_free((void **)split), set_flag(&global->flags.so));
	else if (ft_strcmp(split[0], "WE") == 0)
		return (ft_arr_free((void **)split), set_flag(&global->flags.we));
	else if (ft_strcmp(split[0], "EA") == 0)
		return (ft_arr_free((void **)split), set_flag(&global->flags.ea));
	else if (ft_strcmp(split[0], "F") == 0)
		return (ft_arr_free((void **)split), set_flag(&global->flags.f));
	else if (ft_strcmp(split[0], "C") == 0)
		return (ft_arr_free((void **)split), set_flag(&global->flags.c));
	else
		return (ft_arr_free((void **)split), printf(ERR_NO_ID), false);
}
