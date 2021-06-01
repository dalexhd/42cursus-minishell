#include "../../../includes/minishell.h"

void	ft_slstadd_back(t_slist **alst, t_slist *new)
{
	t_slist	*lst;

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
