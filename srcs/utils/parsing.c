#include "../../includes/minishell.h"

char	**ft_safesplit(t_shell *shell, t_alist *list)
{
	int		size;
	int		i;
	char	**tokens;
	t_args	*c;

	size = ft_alstsize(list);
	tokens = ft_calloc(size + 1, sizeof(char *));
	i = 0;
	while (list)
	{
		c = list->content;
		if (c && !(c->type == R_IN || c->type == R_OUT || c->type == R_AOUT))
		{
			if (!c->is_literal && !ft_strncmp(c->cmd, "$?", 2))
				tokens[i] = ft_itoa(shell->status);
			else
			{
				if (i > 1 && list->prev && list->prev->content->type != FLAG && !c->spaced)
				{
					tokens[i - 1] = ft_strjoin_free(tokens[i - 1], ft_strdup(c->cmd));
					i--;
				}
				else
					tokens[i] = ft_strdup(c->cmd);
			}
			i++;
		}
		list = list->next;
	}
	return (tokens);
}

t_bool	validate_str(t_shell *shell, char *cmd)
{
	int		i;
	t_bool	status;

	i = 0;
	status = true;
	while (i < (int)ft_strlen(cmd))
	{
		if (!valid_redirects(shell, cmd, &i)
			|| !valid_quotes(shell, cmd, &i)
			|| !valid_commas(shell, cmd))
		{
			status = false;
			break ;
		}
		i++;
	}
	return (status);
}

char	*clean_str(t_shell *shell, t_args *arg, char *c)
{
	int		i;
	char	*res;
	char	*status;
	char	*tmp;

	if (arg)
		i = 0;
	res = ft_calloc(ft_strlen(c) + 1, sizeof(char *));
	status = NULL;
	i = 0;
	while (i < (int)ft_strlen(c))
	{
		if (arg->quot_type == N_QUOT && c[i] == '\\')
		{
			ft_strncat(res, &c[i + (ft_strchr("`\\", c[i + 1]) == NULL)], 1);
			i++;
		}
		else if (arg->quot_type == S_QUOT && c[i] == '\\' && c[i + 1] == '\'')
			c[i + 1] = DEL;
		else if ((arg->quot_type != D_QUOT && c[i] == '\\' && c[i + 1] == '\\')
			|| (arg->quot_type == D_QUOT && c[i] == '\\'
				&& ft_strchr("\"\\$`\\", c[i + 1])))
			c[i] = DEL;
		else if (arg->quot_type != S_QUOT && c[i] == '$'
			&& (ft_isalnum(c[i + 1]) || c[i + 1] == '_'))
			parse_dollar(shell, &c, &i, &res);
		else if (arg->quot_type == N_QUOT && c[i] == '~')
			parse_tilde(shell, c, &i, &res);
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
