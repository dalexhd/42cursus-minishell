/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 18:21:08 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/14 17:30:17 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>
#include <string.h>

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \r\n\a\t'\\"

//FIXME: Pipe handling cat /dev/urandom https://github.com/Sisteem/21sh/issues/12

static	void	exec(t_shell *shell, t_parsed *parsed)
{
	if (ft_isbuiltin(parsed->args[0]))
	{
		if (!ft_strcmp(parsed->args[0], "cd"))
			ft_cd(shell, parsed->args[1]);
		else if (!ft_strcmp(parsed->args[0], "export"))
			ft_export(shell, "hola", "mundo");
		else if (!ft_strcmp(parsed->args[0], "env"))
			ft_env(shell);
		else if (!ft_strcmp(parsed->args[0], "pwd"))
			ft_printf("%s\n", ft_pwd());
	}
	else if (execve(builtin_bin_path(shell, parsed->args[0]), parsed->args, shell->envp) == -1)
		ft_error("%s: command not found", 1, parsed->args[0]);
}

t_bool	has_pipe(t_parsed *parsed, char *token)
{
	if (ft_strcmp("|", token) == 0)
	{
		parsed->flags.has_stdout = true;
		return (true);
	}
	if (ft_strcmp(">", token) == 0)
	{
		parsed->flags.redirect.out = true;
		return (true);
	}
	if (ft_strcmp("<", token) == 0)
	{
		parsed->flags.redirect.in = true;
		return (true);
	}
	return (false);
}

void	lsh_split_line(t_shell *shell, char *line)
{
	int bufsize = LSH_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;
	t_parsed *parsed;

	if (!tokens)
	{
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}
	parsed = (t_parsed *)malloc(sizeof(t_parsed));
	parsed->line = ft_strdup(line);
	parsed->flags.has_stdin = false;
	parsed->flags.has_stdout = false;
	parsed->flags.redirect.in = false;
	parsed->flags.redirect.out = false;
	token = strtok(line, LSH_TOK_DELIM);
	while (token != NULL && !has_pipe(parsed, token))
	{
		tokens[position] = token;
		position++;
		if (position >= bufsize)
		{
			bufsize += LSH_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens)
			{
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, LSH_TOK_DELIM);
	}
	tokens[position] = NULL;
	parsed->args = tokens;
	ft_slstadd_back(&shell->parsed, ft_slstnew(parsed));
	if (parsed->flags.has_stdout)
		lsh_split_line(shell, ft_strtrim(ft_substr(parsed->line, token - line + 1, ft_strlen(parsed->line)), " "));
}

static void fill_data(t_slist *list)
{
	while (list->content)
	{
		if (list->prev && list->prev->content->flags.has_stdout)
			list->content->flags.has_stdin = true;
		if (list->next && list->next->content->flags.has_stdin)
			list->content->flags.has_stdout = true;
		if (list->next)
			fill_data(list->next);
		break;
	}
}

static void run(t_shell *shell)
{
	pid_t pid;
	t_slist *list;
	int pipes[2];
	int	input;

	input = 0;
	list = shell->parsed;
	while (list)
	{
		if (ft_strcmp(list->content->args[0], "exit") != 0)
		{
			if (ft_strcmp(list->content->args[0], "cd") == 0)
			{
				exec(shell, list->content);
				list = list->next;
			}
			else
			{
				pipe(pipes);
				pid = fork();
				if (pid < 0)
				{
					ft_error("Fork Failed", 0);
				}
				else if (pid == 0) /* child process */
				{
					dup2(input, STDIN_FILENO);
					if (list->next)
						dup2(pipes[1], STDOUT_FILENO);
					close(pipes[0]);
					exec(shell, list->content);
					exit(0);
				}
				else
				{ /* parent process */
					for (size_t i = 0; i < shell->pipe_count + 1; i++)
						waitpid(-1, NULL, 0);
					close(pipes[1]);
					input = pipes[0];
					list = list->next;
				}
			}
		}
		else
		{
			running = false;
			break ;
		}
	}
}

t_shell *init_shell(char **envp)
{
	t_shell *shell;

	shell = malloc(sizeof(t_shell));
	running = true;
	shell->should_wait = true;
	shell->envp = envp;
	shell->parsed = NULL;
	shell->first = true;
	return (shell);
}

void exec_shell(t_shell *shell, char *cmd)
{
	shell->first = false;
	lsh_split_line(shell, cmd);
	shell->pipe_count = ft_slstsize(shell->parsed);
	fill_data(shell->parsed);
	run(shell);
}
