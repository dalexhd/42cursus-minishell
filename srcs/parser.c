/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 19:49:14 by aborboll          #+#    #+#             */
/*   Updated: 2021/06/01 18:44:48 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_bool	valid_quotes(char *cmd)
{
	return (true);
}

t_list	*parse_args(char *cmd)
{
	t_list		*args;

	args = NULL;
	if (valid_quotes(cmd))
	{
		args = ft_safesplitlist(cmd, ' ', "\"'");
	}
	return (args);
}

char	*parse_line(t_shell *shell, char *cmd)
{
	if (cmd)
		return (clean_str(shell, ft_strdup(cmd)));
	return (cmd);
}
