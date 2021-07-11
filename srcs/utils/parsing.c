#include "../../includes/minishell.h"

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
		if (list->content && !(list->content->type == R_IN
				|| list->content->type == R_OUT
				|| list->content->type == R_AOUT)
		)
		{
			if (!list->content->is_literal
				&& !ft_strncmp(list->content->cmd, "$?", 2))
				tokens[i] = ft_itoa(shell->status);
			else
			{
				if (i > 1 && list->prev->content->type != FLAG && !list->content->spaced)
				{
					tokens[i - 1] = ft_strjoin_free(tokens[i - 1], ft_strdup(list->content->cmd));
					i--;
				}
				else
					tokens[i] = ft_strdup(list->content->cmd);
			}
			i++;
		}
		list = list->next;
	}
	tokens[i] = NULL;
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
			|| !valid_commas(shell, cmd, &i))
		{
			status = false;
			break ;
		}
		i++;
	}
	return (status);
}

char	*clean_str(t_shell *shell, t_args *arg, char *cmd)
{
	int		i;
	char	*res;
	char	*status;

	if (arg)
		i = 0;
	res = ft_strnew(ft_strlen(cmd) + 1);
	i = 0;
	while (i < (int)ft_strlen(cmd))
	{
		if (cmd[i] == '\\' && ft_strchr("\\$\"n", cmd[i + 1]))
		{
			ft_strncat(res, &cmd[i + 1], 1);
			i++;
		}
		else if (arg->quot_type != S_QUOT && cmd[i] == '$'
			&& (ft_isalnum(cmd[i + 1]) || cmd[i + 1] == '_'))
			parse_dollar(shell, &cmd, &i, &res);
		else if (arg->quot_type == N_QUOT && cmd[i] == '~')
			parse_tilde(shell, cmd, &i, &res);
		else if (!ft_strncmp(&cmd[i], "$?", 2))
		{
			status = ft_itoa(shell->status);
			ft_strncat(res, status, ft_strlen(status));
			i++;
		}
		else
			ft_strncat(res, &cmd[i], 1);
		i++;
	}
	res = ft_strdup(res);
	ft_strdel(&cmd);
	return (res);
}
