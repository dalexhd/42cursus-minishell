/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 18:21:08 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/27 17:29:20 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_echo(char **args)
{
	size_t	i;
	t_bool	skip_new_line;

	skip_new_line = false;
	i = 1;
	if (args[1] && !ft_strcmp(args[1], "-n"))
	{
		skip_new_line = true;
		args = &args[1];
	}
	if (!args[1])
		ft_printf("\n");
	while (args[i])
	{
		if (args[i + 1])
			ft_printf("%s ", args[i]);
		else if (skip_new_line)
			ft_printf("%s", args[i]);
		else
			ft_printf("%s\n", args[i]);
		i++;
	}
	return (0);
}
