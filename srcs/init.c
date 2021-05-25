/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 18:21:08 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/25 03:07:16 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	lsh_split_line(t_shell *shell, char *line)
{
	t_list		*tokens;
	t_parsed	*parsed;
	int			i;

	tokens = ft_safesplitlist(line, '|', "\"'");
	while (tokens)
	{
		parsed = (t_parsed *)malloc(sizeof(t_parsed));
		parsed->args = ft_safesplit(tokens->content, ' ', "\"'");
		parsed->line = ft_strdup(line);
		parsed->flags = (t_flags){
			.has_stdin = !!tokens->prev, .has_stdout = !!tokens->next,
			.redirect.in = (t_rstatus){.status = false, .file = NULL},
			.redirect.out = (t_rstatus){.status = false, .file = NULL},
		};
		i = 0;
		while (parsed->args[i])
		{
			if (parsed->args[i][0] == '>')
			{
				parsed->flags.redirect.out = (t_rstatus){.status = true,
					.file = parsed->args[i + 1]};
				parsed->args[i] = NULL;
				parsed->args[i + 1] = NULL;
				break ;
			}
			i++;
		}
		ft_slstadd_back(&shell->parsed, ft_slstnew(parsed));
		tokens = tokens->next;
	}
}

static void	fill_data(t_slist *list)
{
	while (list)
	{
		if (list->prev && list->prev->content->flags.has_stdout)
			list->content->flags.has_stdin = true;
		if (list->next && list->next->content->flags.has_stdin)
			list->content->flags.has_stdout = true;
		if (list->next)
			fill_data(list->next);
		break ;
	}
}

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

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

void	exec_shell(t_shell *shell, char *cmd)
{
	if (ft_strlen(cmd) > 0)
	{
		shell->parsed = NULL;
		shell->first = false;
		lsh_split_line(shell, cmd);
		shell->pipe_count = ft_slstsize(shell->parsed);
		fill_data(shell->parsed);
		run(shell);
	}
	//ft_printf("%i\n", g_running);
	//terminate(shell);
}

//ls | cat -e | "aaa aaaa" | "abaaasssssss" | "aaaaaaa"aaasdasd | sdadaaaawqdwqdqw
//ls | ls | "aaaab" | "abaaasssssss"

//
