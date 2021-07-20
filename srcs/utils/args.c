#include "../../includes/minishell.h"

t_bool	arg_has_red(t_alist *arg)
{
	t_bool	status;

	status = false;
	if (arg->content
		&& (arg->content->type == R_IN
			|| arg->content->type == R_OUT
			|| arg->content->type == R_AOUT))
	{
		status = true;
	}
	return (status);
}

int	get_quote_type(char *cmd)
{
	size_t	len;

	len = ft_strlen(cmd);
	if (cmd[0] == '"' && cmd[len - 1] == '"')
		return (D_QUOT);
	else if (cmd[0] == '\'' && cmd[len - 1] == '\'')
		return (S_QUOT);
	else
		return (N_QUOT);
}

t_bool	arg_is_readable(t_args *content)
{
	return (content->readable);
}

void	filter_readable_args(t_slist *parsed)
{
	if (parsed)
	{
		parsed->content->args = ft_alstfilter(parsed->content->args,
				arg_is_readable, del_alst);
		filter_readable_args(parsed->next);
	}
}
