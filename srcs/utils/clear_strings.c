#include "../../includes/minishell.h"

static void	no_quot(t_shell *shell, t_args *a, char *c, int *i, char *res)
{
	if (a->quot_type == N_QUOT && c[*i] == '\\')
	{
		ft_strncat(res, &c[*i + (ft_strchr("`\\", c[*i + 1]) == NULL)], 1);
		(*i)++;
	}
	else if (a->quot_type == N_QUOT && c[*i] == '~')
		parse_tilde(shell, c, i, &(res));
}

char	*clean_str(t_shell *shell, t_args *a, char *c)
{
	int		i;
	char	*res;
	char	*status;
	char	*tmp;

	res = ft_calloc(ft_strlen(c) + 1, sizeof(char *));
	status = NULL;
	i = 0;
	while (i < (int)ft_strlen(c))
	{
		if (a->quot_type == N_QUOT)// && c[i] == '\\')
			no_quot(shell, a, c, &i, res);
	//	{
	//		ft_strncat(res, &c[i + (ft_strchr("`\\", c[i + 1]) == NULL)], 1);
	//		i++;
	//	}
		else if (a->quot_type == S_QUOT && c[i] == '\\' && c[i + 1] == '\'')
			c[i + 1] = DEL;
		else if ((a->quot_type != D_QUOT && c[i] == '\\' && c[i + 1] == '\\')
			|| (a->quot_type == D_QUOT && c[i] == '\\'
				&& ft_strchr("\"\\$`\\", c[i + 1])))
			c[i] = DEL;
		else if (a->quot_type != S_QUOT && c[i] == '$'
			&& (ft_isalnum(c[i + 1]) || c[i + 1] == '_'))
			parse_dollar(shell, &c, &i, &res);
//		else if (a->quot_type == N_QUOT && c[i] == '~')
//			parse_tilde(shell, c, &i, &res);
		else if (!ft_strncmp(&c[i], "$?", 2))
		{
			status = ft_itoa(shell->status);
			ft_strncat(res, status, ft_strlen(status));
			i++;
		}
		else
			ft_strncat(res, &c[i], 1);
		i++;
	}
	if (status)
		ft_strdel(&status);
	tmp = ft_strdup(res);
	ft_strdel(&res);
	return (tmp);
}
