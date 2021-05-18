/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/18 18:01:05 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/18 20:42:50 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	void	split_tokens(t_shell *shell, char *line)
{

}

static	t_bool	validate_quotes(char *cmd)
{
	size_t	i;
	size_t	quotes;

	i = 0;
	quotes = 0;
	while (cmd[i])
	{
		if (cmd[i] == '"')
			quotes++;
		i++;
	}
	return (quotes % 2 == 0);
}

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
	t_bool	valid_quotes;
	char	*line;

	line = ft_strnew(ft_strlen(cmd));
	valid_quotes = validate_quotes(cmd);
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\\' && cmd[i + 1] != '\\' && valid_quotes) // If current char is '\\' and next char isn´t '\\'
		{
			ft_strncat(line, &cmd[i], 1);
		}
		else if (cmd[i] == '$' && cmd[i - 1] != '\\')
		{
			parse_dollar(shell, cmd, &i, line);
		}
		else if (cmd[i] == '~' && cmd[i - 1] != '\\')
		{
			parse_tilde(shell, cmd, &i, line);
		} else {
			ft_strncat(line, &cmd[i], 1);
		}
		i++;
	}
	ft_printf("Valid quotes: %i\n", valid_quotes);
	ft_printf("Cmd: %s\n", line);
}

void	parse_line(t_shell *shell, char *cmd)
{
	char	*parsed;

	parsed = clean_str(shell, ft_strtrim(ft_strdup(cmd), " "));
	//split_tokens(shell, parsed);
}
//echo HOME | cat -e $USER | cd ~/Desktop
