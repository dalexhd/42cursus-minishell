/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 18:21:08 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/18 17:37:33 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_env(t_shell *shell)
{
	int		i;

	i = 0;
	while (shell->envp[i] != 0)
	{
		ft_printf("%s\n", shell->envp[i]);
		i++;
	}
}

/*
* Get an environment variable
*/
char	*ft_getenv(t_shell *shell, char *env)
{
	int	i;

	i = 0;
	while (shell->envp[i] != 0)
	{
		if (ft_strncmp(shell->envp[i], env, ft_strlen(env)) == 0)
			return (ft_substr(shell->envp[i], ft_strlen(env) + 1,
					ft_strlen(shell->envp[i])));
		i++;
	}
	return (NULL);
}
