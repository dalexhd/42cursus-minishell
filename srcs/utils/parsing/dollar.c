#include "../../../includes/minishell.h"

static t_bool	ft_isenv(int c)
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
