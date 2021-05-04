/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 18:21:08 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/04 21:35:02 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define NUMBER_OF_STRING 4
#define MAX_STRING_SIZE 40

void	test_builtins(t_shell shell)
{
	char *array[] = {
		"hello world"
	};
	ft_echo(array, sizeof(array) / sizeof(char *));
	ft_printf("\n");
	ft_pwd();
	ft_printf("\n");
	ft_env(shell.envp);
	ft_printf("\n");
	ft_cd("../", shell.envp);
	ft_printf("\n");
	ft_pwd();
}

t_shell	init_shell(char **envp)
{
	t_shell	shell;

	shell.running = true;
	shell.envp = envp;
	test_builtins(shell);
	return (shell);
}
