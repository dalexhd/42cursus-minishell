/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 18:21:08 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/19 23:46:16 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>
#include <string.h>

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \r\n\a\t'\\"

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

t_pipes	*locate_pipes(char *line)
{
	t_pipes	*pipes;
	size_t	i;
	t_bool	open_quote;
	t_bool	open_dquote;

	i = 0;
	open_quote = false;
	open_dquote = false;
	pipes = (t_pipes *)malloc(sizeof(t_pipes));
	pipes->count = 0;
	pipes->pos = NULL;
	while (line[i])
	{
		char a = line[i];
		if (line[i] == '"')
			open_quote = !open_quote;
		if (line[i] == '\'')
			open_dquote = !open_dquote;
		if (!open_quote && !open_dquote && line[i] == '|')
		{
			pipes->count++;
			ft_lstadd_back(&pipes->pos, ft_lstnew(i));
		}
		i++;
	}
	return (pipes);
}

void	lsh_split_line2(t_shell *shell, char *line)
{
	t_pipes	*pipes;
	t_list	*list;
	size_t	i;
	size_t	j;
	size_t	u;
	t_list	*tokens;

	i = 0;
	i = 0;
	u = ft_strlen(line);
	pipes = locate_pipes(line);
	list = pipes->pos;
	ft_lstadd_back(&tokens, ft_lstnew(ft_substr(line, 0, list->content)));
	while (i < pipes->count)
	{
		j = 0;
		if (list->prev)
		{
			j = list->content - list->prev->content;
			if (!list->next)
				j = u;
		}
		else if (list->next)
			j = list->next->content - list->content;
		else
		{
			j = u;
		}
		ft_lstadd_back(&tokens, ft_lstnew(ft_substr(line, list->content, j)));
		list = list->next;
		i++;
	}
	i = 0;
	while (tokens)
	{
		ft_printf("Token[%i] => %s\n", i, tokens->content);
		tokens = tokens->next;
		i++;
	}
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

static	void	run(t_shell *shell)
{
	pid_t	pid;
	pid_t	*pids;
	t_slist	*list;
	int		**pipes;
	int input, i;

	input = 0;
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
	while (list)
	{
		if (ft_strcmp(list->content->args[0], "exit") == 0)
		{
			g_running = false;
			break ;
		}
		else if (ft_strcmp(list->content->args[0], "cd") == 0)
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
				if (list->content->flags.has_stdin)
				{
					dup2(input, STDIN_FILENO);
					if (list->content->flags.has_stdout)
						dup2(input, STDOUT_FILENO);
					else
						exec(shell, list->content);
				}
				else if (list->content->flags.has_stdout)
				{
					dup2(input, STDOUT_FILENO);
					exec(shell, list->content);
				}
				else
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
					if (list->content->flags.has_stdin)
						dup2(input, STDIN_FILENO);
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
					if (list->content->flags.has_stdout)
						dup2(input, STDOUT_FILENO);
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

t_shell *init_shell(char **envp)
{
	t_shell *shell;

	shell = malloc(sizeof(t_shell));
	g_running = true;
	shell->should_wait = true;
	shell->envp = envp;
	shell->parsed = NULL;
	shell->first = true;
	return (shell);
}

static	void	*terminate(t_shell *shell)
{
	t_slist *list;

	list = shell->parsed;
	while (list)
	{
		waitpid(list->content->pid, NULL, 0);
		list = list->next;
	}
	list = shell->parsed;
}

void exec_shell(t_shell *shell, char *cmd)
{
	int				exit_status;

	shell->parsed = NULL;
	shell->first = false;
	lsh_split_line2(shell, cmd);
/* 	shell->pipe_count = ft_slstsize(shell->parsed);
	fill_data(shell->parsed);
	run(shell);
	terminate(shell); */
}

//ls | ls | "a" | "abaaa" | "aaaaaaa"aaasdasd | sdadaaaawqdwqdqw
