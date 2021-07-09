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

static	int	filtering_args(t_alist **tmp, t_alist *args)
{
	if (!args)
		return (0);
	else if (args->content->readable)
		ft_alstadd_back(&(*tmp), ft_alstnew(args->content));
	return (filtering_args(tmp, args->next));
}

void	filter_readable_args(t_slist **parsed)
{
	t_alist	*tmp;

	if (*parsed)
	{
		tmp = (t_alist *)malloc(sizeof(t_alist));
		tmp = NULL;
		while (1)
		{
			if (!filtering_args(&tmp, (*parsed)->content->args))
				break ;
		}
		(*parsed)->content->args = tmp;
		filter_readable_args(&(*parsed)->next);
	}
}
