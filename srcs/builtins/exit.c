/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/27 16:09:16 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/27 16:56:39 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_exit(char **args)
{
	size_t	i;

	i = 1;
	while (args[i])
	{
		if (args[i + 1])
		{
			ft_error("minishell: exit: too many arguments\n", 1);
			return ;
		}
		else if ((args[1][0] == '-' && !ft_strevery(&args[1][1], ft_isdigit))
			|| (args[1][0] != '-' && !ft_strevery(args[1], ft_isdigit)))
		{
			ft_error("minishell: exit: %s: numeric argument required\n", 0, args[i]);
			exit(255);
		}
		else
		{
			g_running = false;
			exit(ft_atoi(args[1]));
		}
		i++;
	}
}
