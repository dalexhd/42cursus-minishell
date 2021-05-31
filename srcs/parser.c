/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 19:49:14 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/31 18:50:00 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	void	parse_dollar(t_shell *shell, char *cmd, size_t *i, char *line)
{
	char	*env;
	char	*tmp;

	tmp = ft_strnew(1);
	while (cmd[*i] && !ft_isspace(cmd[*i]))
	{
		if (cmd[*i] != '$')
			ft_strncat(tmp, &cmd[*i], 1);
		(*i)++;
	}
	(*i)--;
	env = ft_getenv(shell, tmp);
	if (env)
		ft_strcat(line, env);
}

static	void	parse_tilde(t_shell *shell, char *cmd, size_t *i, char *line)
{
	char	*env;
	char	*tmp;

	tmp = ft_strnew(1);
	env = ft_getenv(shell, "HOME");
	if (env)
		ft_strcat(tmp, env);
	while (cmd[*i] && !ft_isspace(cmd[*i]))
	{
		if (cmd[*i] != '~')
			ft_strncat(tmp, &cmd[*i], 1);
		(*i)++;
	}
	(*i)--;
	ft_strcat(line, tmp);
}

static	char	*clean_str(t_shell *shell, char *cmd)
{
	size_t	i;
	char	*line;
	t_bool	test;

	line = ft_strnew(ft_strlen(cmd));
	test = true;
	i = 0;
	while (cmd[i])
	{
		if (i > 0 && cmd[i - 1] == '\\')
			test = false;
		if (cmd[i] == '\\' && cmd[i + 1] != '\\') // If current char is '\\' and next char isn´t '\\'
			ft_strncat(line, &cmd[i], 1);
		else if (cmd[i] == '$' && test)
			parse_dollar(shell, cmd, &i, line);
		else if (cmd[i] == '~' && test)
			parse_tilde(shell, cmd, &i, line);
		else
			ft_strncat(line, &cmd[i], 1);
		i++;
	}
	return (line);
}

char	*parse_line(t_shell *shell, char *cmd)
{
	if (cmd)
		return (clean_str(shell, ft_strdup(cmd)));
	return (cmd);
}
