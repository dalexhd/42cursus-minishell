#include "../../includes/minishell.h"

static	t_bool	clean_str_sec(t_args *arg, int *i, char **res, char **c)
{
	t_bool		ret;

	ret = false;
	if (arg->quot_type == N_QUOT && (*c)[*i] == '\\')
	{
		ft_strncat(*res,
			&(*c)[*i + (ft_strchr("`\\", (*c)[*i + 1]) == NULL)], 1);
		(*i)++;
		ret = true;
	}
	else if (arg->quot_type == S_QUOT
		&& (*c)[*i] == '\\' && (*c)[*i + 1] == '\'')
	{
		(*c)[*i + 1] = DEL;
		ret = true;
	}
	else if ((arg->quot_type != D_QUOT && (*c)[*i] == '\\'
		&& (*c)[*i + 1] == '\\')
		|| (arg->quot_type == D_QUOT && (*c)[*i] == '\\'
			&& ft_strchr("\"\\$`\\", (*c)[*i + 1])))
	{
		(*c)[*i] = DEL;
		ret = true;
	}
	return (ret);
}

static	void	clean_str_dollar(t_shell *shell, int *i, char **res)
{
	char	*status;

	status = ft_itoa(shell->status);
	ft_strncat(*res, status, ft_strlen(status));
	ft_strdel(&status);
	(*i)++;
}

char	*clean_str(t_shell *shell, t_args *arg, char *c)
{
	int		i;
	char	*res;

	res = ft_calloc(ft_strlen(c) + 1, sizeof(char *));
	i = 0;
	while (i < (int)ft_strlen(c))
	{
		if (!clean_str_sec(arg, &i, &res, &c))
		{
			if (arg->quot_type != S_QUOT && c[i] == '$'
				&& (ft_isalnum(c[i + 1]) || c[i + 1] == '_'))
				parse_dollar(shell, &c, &i, &res);
			else if (arg->quot_type == N_QUOT && c[i] == '~')
				parse_tilde(shell, c, &i, &res);
			else if (!ft_strncmp(&c[i], "$?", 2))
				clean_str_dollar(shell, &i, &res);
			else
				ft_strncat(res, &c[i], 1);
		}
		i++;
	}
	return (ft_strdupdel(res));
}
