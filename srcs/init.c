/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 18:21:08 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/10 20:06:43 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	test_extbuiltin(t_shell shell)
{
	pid_t	pid;
	int		fd[2];

	char *args[] = {"ls", "-l", "-a", (char *)0};
	char *grepArgs[] = {"grep", "Makefile", (char *)0};

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
		if (execve("/bin/ls", args, shell.envp) == -1)
			ft_error("permission denied", 1);
	}
	else if (pid < 0)
		ft_error("failed to fork", 1);
	else
	{
		pid = fork();
        if(pid == 0)
        {
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
			if (execve("/usr/bin/grep", grepArgs, shell.envp) == -1)
				ft_error("permission denied", 1);
        }
	}
}

void	test_intextbuiltin(t_shell shell)
{
	pid_t	pid;
	int		fd[2];

	char *grepArgs[] = {"grep", "HOME", (char *)0};

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
		ft_env(shell);
	}
	else if (pid < 0)
		ft_error("failed to fork", 1);
	else
	{
		pid = fork();
        if(pid == 0)
        {
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
			if (execve("/usr/bin/grep", grepArgs, shell.envp) == -1)
				ft_error("permission denied", 1);
        }
	}
}

void	test_builtins(t_shell shell)
{
/* 	char	*array[] = {
		"hello world"
	};
	ft_echo(array, sizeof(array) / sizeof(char *)); */
	char	*test;

	ft_printf("\n");
	test = ft_pwd();
	ft_printf("Current directory: %s\n", test);
	ft_strdel(&test);
	test = ft_getenv(shell, "PWD");
	ft_printf("PWD env value=%s\n", test);
	ft_strdel(&test);
	test = ft_getenv(shell, "OLDPWD");
	ft_printf("OLDPWD env value=%s\n", test);
	ft_strdel(&test);
	ft_info("Lets move directory ../", false);
	ft_cd(shell, "../");
	test = ft_pwd();
	ft_printf("Current directory: %s\n", test);
	ft_strdel(&test);
	test = ft_getenv(shell, "PWD");
	ft_printf("PWD env value=%s\n", test);
	ft_strdel(&test);
	test = ft_getenv(shell, "OLDPWD");
	ft_printf("OLDPWD env value=%s\n", test);
	ft_strdel(&test);
	ft_info("Lets unset HOME env variable", false);
	ft_unset(shell, "HOME");
	test = ft_getenv(shell, "HOME");
	ft_printf("HOME env value=%s\n", test);
	ft_strdel(&test);
}

t_shell	init_shell(char **envp)
{
	t_shell	shell;
	int		i;

	i = 0;
	shell.running = true;
	while (envp[i] != 0)
	{
		shell.envp[i] = ft_strdup(envp[i]);
		i++;
	}
	//test_builtins(shell);
	test_extbuiltin(shell);
	test_intextbuiltin(shell);
	return (shell);
}

//ft_lineexec("export HOLA=adios && env | grep HOLA");