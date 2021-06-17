#include "../../includes/minishell.h"

t_bool	ft_isenv(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9') || (c == '_'))
	{
		return (true);
	}
	return (false);
}

void	parse_dollar(t_shell *shell, char **cmd, int *i, char *line)
{
	char	*env;
	char	*tmp;
	char	*aux;

	tmp = ft_strnew(1);
	if (ft_isdigit(ft_strdup(*cmd)[*i + 1]))
	{
		*(cmd) += 2;
		(*i)--;
		return ;
	}
	aux = ft_strdup(*cmd);
	(*i)++;
	while (aux[*i] && ft_isenv(aux[*i]))
	{
		if (aux[*i] != '$')
			ft_strncat(tmp, &aux[*i], 1);
		(*i)++;
	}
	(*i)--;
	env = ft_getenv(shell, tmp);
	if (env)
		ft_strcat(line, env);
}

char	**ft_safesplit(t_shell *shell, t_alist *list)
{
	int		size;
	int		i;
	char	**tokens;

	size = ft_alstsize(list);
	tokens = (char **)malloc(sizeof(char *) * (size + 1));
	i = 0;
	while (list)
	{
		if (list->content)
		{
			if (!list->content->is_literal
				&& !ft_strcmp(list->content->cmd, "$?"))
				tokens[i] = ft_itoa(shell->exit_status);
			else
				tokens[i] = list->content->cmd;
			i++;
		}
		list = list->next;
	}
	tokens[i] = NULL;
	return (tokens);
}

void	parse_tilde(t_shell *shell, char *cmd, int *i, char *line)
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

char	*clean_str(t_shell *shell, t_args *arg, char *cmd)
{
	int	i;
	char	*res;
	t_bool	test;

	res = ft_strnew(ft_strlen(cmd));
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\\' && ft_strchr("\\$\"n", cmd[i + 1]))
		{
			ft_strncat(res, &cmd[i + 1], 1);
			i++;
		}
		else if (!arg->is_literal && cmd[i] == '$' && (ft_isalpha(cmd[i + 1])
			|| cmd[i + 1] == '_') && test)
			parse_dollar(shell, &cmd, &i, res);
		else if (cmd[i] == '~' && test)
			parse_tilde(shell, cmd, &i, res);
		else
			ft_strncat(res, &cmd[i], 1);
		i++;
	}
	return (res);
}
