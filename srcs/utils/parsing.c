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

t_bool	validate_str(t_shell *shell, char *cmd)
{
	size_t	i;
	t_bool	status;

	i = 0;
	status = true;
	while (i < ft_strlen(cmd))
	{
		if (!valid_redirects(shell, cmd, &i)
			|| !valid_quotes(shell, cmd, &i))
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

	res = ft_strnew(ft_strlen(cmd));
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\\' && ft_strchr("\\$\"n", cmd[i + 1]))
		{
			ft_strncat(res, &cmd[i + 1], 1);
			i++;
		}
		else if (!arg->is_literal && cmd[i] == '$'
			&& (ft_isalpha(cmd[i + 1]) || cmd[i + 1] == '_'))
			parse_dollar(shell, &cmd, &i, res);
		else if (cmd[i] == '~')
			parse_tilde(shell, cmd, &i, res);
		else
			ft_strncat(res, &cmd[i], 1);
		i++;
	}
	return (res);
}
