#include "../../../includes/minishell.h"

void	ft_rlstadd_back(t_rlist **alst, t_rlist *new)
{
	t_rlist	*lst;

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
