/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 18:21:08 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/05 21:30:55 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
* Set an environment variable
*/
void	ft_export(t_shell shell, char *env, char *value)
{
	int	i;

	i = 0;
	while (shell.envp[i] != 0)
	{
		if (ft_strncmp(shell.envp[i], env, ft_strlen(env)) == 0)
		{
			shell.envp[i] = ft_strjoin(ft_strjoin(env, "="), value);
			return ;
		}
		i++;
	}
	shell.envp[i] = ft_strjoin(ft_strjoin(env, "="), value);
}
