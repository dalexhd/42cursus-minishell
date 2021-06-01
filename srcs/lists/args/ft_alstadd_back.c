#include "../../../includes/minishell.h"

void	ft_alstadd_back(t_alist **alst, t_alist *new)
{
	t_alist	*lst;

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
