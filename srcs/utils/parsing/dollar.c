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

static	void	parse_aux(char *aux, char **line, char *tmp)
{
	if (aux)
		*line = ft_strjoin_free(*line, ft_strdup(aux));
	else if (ft_isdigit(tmp[0]))
		*line = ft_strjoin_free(*line, ft_strcut(tmp, 2, ft_strlen(tmp)));
	else if (!aux)
		*line = ft_strjoin_free(*line, ft_strdup("\177"));
}

void	parse_dollar(t_shell *shell, char **cmd, int *i, char **line)
{
	char	*tmp;
	char	*aux;

	if (ft_isdigit((*cmd)[*i + 1]))
	{
		*cmd = &(*cmd)[2];
		(*i)--;
		return ;
	}
	tmp = ft_strnew(1);
	aux = ft_strdup(*cmd);
	(*i)++;
	while (aux[*i] && ft_isenv(aux[*i]))
	{
		if (aux[*i] != '$')
			ft_strncat(tmp, &aux[*i], 1);
		(*i)++;
	}
	(*i)--;
	ft_strdel(&aux);
	parse_aux(ft_getenv(shell, tmp, false), line, tmp);
	ft_strdel(&tmp);
}
