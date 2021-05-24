/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 18:21:08 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/24 14:56:28 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>

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
		if (line[i] == '"')
			open_quote = !open_quote;
		if (line[i] == '\'')
			open_dquote = !open_dquote;
		if (!open_quote && !open_dquote && line[i] == '|')
		{
			pipes->count++;
			ft_lstadd_back(&pipes->pos, ft_lstnew((void *)i));
		}
		i++;
	}
	return (pipes);
}

void	lsh_split_line2(t_shell *shell, char *line)
{
	size_t	i;
	t_list	*tokens;

	tokens = NULL;
	char **pipeparts = ft_safesplit(line, '|', "\"'\\");
	i = 0;
	while (pipeparts[i] != NULL)
	{
		if (i > 0)
			ft_memmove(pipeparts[i], pipeparts[i] + 1, ft_strlen(pipeparts[i]));
		char *asda = ft_strtrim(pipeparts[i], " ");
		ft_printf("%s\n", asda);
		ft_lstadd_back(&tokens, ft_lstnew(asda));
		i++;
	}
	while (tokens)
	{
		t_parsed *parsed;
		char **parts = ft_safesplit(tokens->content, ' ', "\"'");
		parsed = (t_parsed *)malloc(sizeof(t_parsed));
		parsed->line = ft_strdup(line);
		parsed->flags.has_stdin = tokens->prev != NULL;
		parsed->flags.has_stdout = tokens->next != NULL;
		parsed->flags.redirect.in = false;
		parsed->flags.redirect.out = false;
		parsed->args = parts;
		ft_slstadd_back(&shell->parsed, ft_slstnew(parsed));
		tokens = tokens->next;
	}
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
	init_tc(shell);
	return (shell);
}

/* static	void	*terminate(t_shell *shell)
{
	t_slist *list;

	list = shell->parsed;
	while (list)
	{
		waitpid(list->content->pid, NULL, 0);
		list = list->next;
	}
	list = shell->parsed;
} */

void exec_shell(t_shell *shell, char *cmd)
{
	shell->parsed = NULL;
	shell->first = false;
	lsh_split_line2(shell, cmd);
/* 	shell->pipe_count = ft_slstsize(shell->parsed);
	fill_data(shell->parsed);*/
	run(shell);
	//ft_printf("%i\n", g_running);
	//terminate(shell);
}

//ls | cat -e | "aaaaaaa" | "abaaasssssss" | "aaaaaaa"aaasdasd | sdadaaaawqdwqdqw
//ls | ls | "aaaab" | "abaaasssssss"
