#include "../../../includes/minishell.h"

void	ft_hlstadd_back(t_hist **alst, t_hist *new)
{
	t_hist	*lst;

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
