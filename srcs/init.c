/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 18:21:08 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/18 22:21:16 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** Here we initialize the bash shell. We define the variables
** g_running global variable and we assign the shell pointer.
*/
t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	g_running = true;
	shell->envp = envp;
	shell->parsed = NULL;
	shell->first = true;
	return (shell);
}

void	exec_shell(t_shell *shell, char *cmd)
{
	char	*parsed;

	(void)*shell;
	split_tokens(shell, cmd);
	parse_line(shell, cmd);
	ft_printf("%s\n", cmd);
	shell->first = false;
}
