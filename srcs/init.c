/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 18:21:08 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/24 20:49:37 by aborboll         ###   ########.fr       */
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

void	lsh_split_line(t_shell *shell, char *line)
{
	t_list		*tokens;
	t_parsed	*parsed;
	char		**parts;

	tokens = ft_safesplitlist(line, '|', "\"'");
	while (tokens)
	{
		parts = ft_safesplit(tokens->content, ' ', "\"'");
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
	lsh_split_line(shell, cmd);
 	shell->pipe_count = ft_slstsize(shell->parsed);
	fill_data(shell->parsed);
	run(shell);
	//ft_printf("%i\n", g_running);
	//terminate(shell);
}

//ls | cat -e | "aaa aaaa" | "abaaasssssss" | "aaaaaaa"aaasdasd | sdadaaaawqdwqdqw
//ls | ls | "aaaab" | "abaaasssssss"

//
