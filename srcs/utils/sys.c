/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 18:06:58 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/27 19:04:36 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exec(t_shell *shell, t_parsed *parsed)
{
	char	*arg;
	char	**args;

	arg = (char *)parsed->args->content;
	args = ft_safesplit(parsed->args);
	if (ft_isbuiltin(arg))
	{
		if (!ft_strcmp(arg, "cd"))
			ft_cd(shell, parsed->args->next->content);
		else if (!ft_strcmp(arg, "exit"))
			ft_exit(args);
		else if (!ft_strcmp(arg, "export"))
			ft_export(shell, args);
		else
		{
			if (!ft_strcmp(arg, "env"))
				ft_env(shell);
			else if (!ft_strcmp(arg, "pwd"))
				ft_printf("%s\n", ft_pwd());
			else if (!ft_strcmp(arg, "echo"))
				ft_echo(args);
			exit(0);
		}
	}
	else if (execve(builtin_bin_path(shell, args[0]), args, shell->envp) == -1)
		ft_error("%s: command not found\n", 1, arg);
}

static void	handle_redirect(t_slist	*list)
{
	int input;
	t_rlist *redirect;

	redirect = list->content->redirects;
	while (redirect)
	{
		if (redirect->content->in.status)
		{
			input = open(redirect->content->in.file, O_RDONLY, 0600);
			if (input < 0)
				return ft_error("bash: %s: %s\n", true, redirect->content->in.file, strerror(errno));
			dup2(input, STDIN_FILENO);
		}
		else if (redirect->content->out.status)
		{
			input = open(redirect->content->out.file, O_TRUNC | O_WRONLY | O_CREAT, 0600);
			if (input < 0)
				return ft_error("bash: %s: %s\n", true, redirect->content->out.file, strerror(errno));
			dup2(input, STDOUT_FILENO);
		}
		else if (redirect->content->aout.status)
		{
			input = open(redirect->content->aout.file, O_WRONLY | O_CREAT | O_APPEND, 0600);
			if (input < 0)
				return ft_error("bash: %s: %s\n", true, redirect->content->aout.file, strerror(errno));
			dup2(input, STDOUT_FILENO);
		}
		redirect = redirect->next;
	}
}

void	run(t_shell *shell)
{
	pid_t	pid;
	pid_t	*pids;
	t_slist	*list;
	int		**pipes;
	int input, i, s;

	list = shell->parsed;
	pids = malloc(ft_slstsize(shell->parsed) * sizeof(int) + 1);
	pipes = (int **) malloc ((ft_slstsize(shell->parsed) - 1) * sizeof(int *));
	for(int i = 0; i < ft_slstsize(shell->parsed) - 1; i++)
	{
			pipes[i] = (int *) malloc (2 * sizeof(int));
			if(pipe(pipes[i]) < 0)
				ft_error("Fallo al crear el pipe %s/n", 1, strerror(errno));
	}
	i = 0;
	s = 0;
	while (list)
	{
		input = 0;
		if (ft_strcmp(list->content->args->content, "exit") == 0)
		{
			exec(shell, list->content);
		}
		else if (ft_strcmp(list->content->args->content, "cd") == 0)
		{
			exec(shell, list->content);
		}
		else if (ft_slstsize(shell->parsed) == 1) //If there is only a single command
		{
			pid = fork();
			if(pid < 0)
				ft_error("Fallo el fork() %s\n", 1, strerror(errno));
			else if (pid == 0) // Hijo
			{
				handle_redirect(list);
				exec(shell, list->content);
			}
			else
				waitpid(pid, NULL, 0); // Wait for the children
		}
		else
		{
			pid = fork();
			if(pid < 0)
				ft_error("Fallo el fork() %s\n", 1, strerror(errno));
			else if (pid == 0) // Children
			{
				if (i == 0)
				{
					if (list->content->flags.redirect.in.status)
					{
						input = open(list->content->flags.redirect.in.file, O_RDONLY, 0600);
						if (input < 0)
							return ft_error("bash: %s: %s\n", true, list->content->flags.redirect.in.file, strerror(errno));
						dup2(input, STDIN_FILENO);
					}
					for(int j = 1; j < ft_slstsize(shell->parsed) - 1; j++)
					{
						close(pipes[j][1]);
						close(pipes[j][0]);
					}
					close(pipes[0][0]);
					dup2(pipes[0][1], 1);
				}
				if (i > 0 && i < ft_slstsize(shell->parsed) - 1)
				{
					if (i == 1 && ft_slstsize(shell->parsed) != 3)
					{
						for(int j = i + 1; j < ft_slstsize(shell->parsed) - 1; j++)
						{
							close(pipes[j][1]);
							close(pipes[j][0]);
						}
					}
					if (i == ft_slstsize(shell->parsed) - 2 && ft_slstsize(shell->parsed) != 3)
					{
						for(int j = 0; j < i - 1; j++)
						{
							close(pipes[j][1]);
							close(pipes[j][0]);
						}
					}
					if (i != 1 && i != ft_slstsize(shell->parsed) -2  && ft_slstsize(shell->parsed) != 3)
					{
						for(int j = 0; j < i - 1; j++)
						{
							close(pipes[j][1]);
							close(pipes[j][0]);
						}
						for(int j = i + 1; j < ft_slstsize(shell->parsed) - 1; j++)
						{
							close(pipes[j][1]);
							close(pipes[j][0]);
						}
					}
					close(pipes[i - 1][1]);
					dup2(pipes[i - 1][0], 0);
					close(pipes[i][0]);
					dup2(pipes[i][1], 1);
				}
				if (i == ft_slstsize(shell->parsed) - 1)
				{
					if (list->content->flags.redirect.out.status)
					{
						input = open(list->content->flags.redirect.out.file, O_TRUNC | O_WRONLY | O_CREAT, 0600);
						if (input < 0)
							return ft_error("bash: %s: %s\n", true, list->content->flags.redirect.out.file, strerror(errno));
						dup2(input, STDOUT_FILENO);
					}
					else if (list->content->flags.redirect.aout.status)
					{
						input = open(list->content->flags.redirect.aout.file, O_WRONLY | O_APPEND, 0600);
						if (input < 0)
							return ft_error("bash: %s: %s\n", true, list->content->flags.redirect.aout.file, strerror(errno));
						dup2(input, STDOUT_FILENO);
					}
					for(int j = 0; j < ft_slstsize(shell->parsed) - 2; j++)
					{
						close(pipes[j][1]);
						close(pipes[j][0]);
					}
					close(pipes[i - 1][1]);
					dup2(pipes[i - 1][0], 0);
				}
				exec(shell, list->content);
			}
			else // Parent
			{
				pids[i] = pid; // We save the PID of the children
			}
		}
		list = list->next;
		i++;
	}
	for(int k = 0; k < ft_slstsize(shell->parsed) - 1; k++)
	{
		close(pipes[k][1]);
		close(pipes[k][0]);
	}
	for(int k = 0; k < ft_slstsize(shell->parsed); k++)
	{
		waitpid(pids[k], NULL, 0);
	}
	for(int i = 0; i < ft_slstsize(shell->parsed) - 1; i++)
	{
		free(pipes[i]);
	}
	free(pipes);
	free(pids);
}
