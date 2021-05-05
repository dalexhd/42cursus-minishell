/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 18:21:08 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/05 21:44:03 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	test_builtins(t_shell shell)
{
	char	*array[] = {
		"hello world"
	};
	ft_echo(array, sizeof(array) / sizeof(char *));
	ft_printf("\n");
	ft_printf("Current directory: %s\n", ft_pwd());
	ft_printf("PWD env value=%s\n", ft_getenv(shell, "PWD"));
	ft_printf("OLDPWD env value=%s\n", ft_getenv(shell, "OLDPWD"));
	ft_info("Lets move directory ../", false);
	ft_cd(shell, "../");
	ft_printf("Current directory: %s\n", ft_pwd());
	ft_printf("PWD env value=%s\n", ft_getenv(shell, "PWD"));
	ft_printf("OLDPWD env value=%s\n", ft_getenv(shell, "OLDPWD"));
	ft_info("Lets unset HOME env variable", false);
	ft_unset(shell, "HOME");
	ft_printf("HOME env value=%s\n", ft_getenv(shell, "HOME"));
}

t_shell	init_shell(char **envp)
{
	t_shell	shell;

	shell.running = true;
	shell.envp = envp;
	test_builtins(shell);
	return (shell);
}
