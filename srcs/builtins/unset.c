/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 18:21:08 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/11 21:13:13 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
* Unset an environment variable
*/
void	ft_unset(t_shell *shell, char *env)
{
	int	i;
	int	u;

	i = 0;
	u = 0;
	while (shell->envp[i] != 0)
	{
		if (ft_strncmp(shell->envp[i], env, ft_strlen(env)) != 0)
		{
			shell->envp[u] = shell->envp[i];
			u++;
		}
		i++;
	}
}
