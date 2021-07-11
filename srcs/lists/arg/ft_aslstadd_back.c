#include "../../../includes/minishell.h"

void	ft_aslstadd_back(t_aslist **alst, t_aslist *new)
{
	t_aslist	*lst;

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
