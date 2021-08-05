#include "../../../includes/minishell.h"

static void	quotes(char *cmd, int *i, char c)
{
	while (cmd[*i] && cmd[*i] != c)
		(*i)++;
	if (!cmd[*i])
		*i = -42;
}

static t_bool	check_error(t_shell *shell, char *cmd, int *pos, char one)
{
	if (one == '\'' || one == '"')
	{
		sh_error(shell, ERR_UQ, 2, *pos, cmd + *pos);
		shell->parsed->content->valid = false;
		return (false);
	}
	return (true);
}

static void	check_var(t_shell *shell, char **cmd, int *i)
{
	size_t		e;
	char		*env;
	char		*value;
	char		*tmp;

	e = 1;
	while (e < ft_strlen(&(*cmd)[*i]) + 1)
	{
		if (ft_strchr(" -./$'\"\"\0", (*cmd)[*i + e]))
		{
			env = ft_strcut(&(*cmd)[*i], 1, e - 1);
			value = ft_getenv(shell, env, false);
			if (ft_getenv(shell, env, false))
			{
				tmp = ft_strjoin_free(ft_strcut(*cmd, 0, *i),
						ft_strjoin(ft_getenv(shell, env, 0), &(*cmd)[*i + e]));
				ft_strdel(&(*cmd));
				*cmd = tmp;
				*i += ft_strlen(value) - 1;
			}
			ft_strdel(&env);
			break ;
		}
		e++;
	}
}

t_bool	valid_quotes(t_shell *shell, char **cmd, int *i)
{
	char	one;
	int		pos;

	if (((*cmd)[*i] == '\'' || (*cmd)[*i] == '"') && (*cmd)[*i])
	{
		if (*i > 0 && (*cmd)[*i - 1] == '\\')
		{
			(*i)++;
			return (true);
		}
		one = (*cmd)[*i];
		pos = (*i)++;
		quotes(*cmd, i, one);
		if (*i == -42)
			return (check_error(shell, *cmd, &pos, one));
		else if ((*cmd)[*i] == one)
			one = 0;
	}
	if ((*cmd)[*i] == '$' && (*cmd)[*i + 1] != '?')
		check_var(shell, &(*cmd), i);
	return (true);
}
