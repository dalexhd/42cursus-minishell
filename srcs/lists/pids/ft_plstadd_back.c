#include "../../../includes/minishell.h"

void	ft_plstadd_back(t_pids **alst, t_pids *new)
{
	t_pids	*lst;

	if (alst && new)
	{
		if (!(*alst))
			*alst = new;
		else
		{
			lst = *alst;
			while (lst->next)
				lst = lst->next;
			lst->next = new;
			new->prev = lst;
		}
	}
}
