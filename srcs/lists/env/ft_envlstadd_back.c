#include "../../../includes/minishell.h"

void	ft_envlstadd_back(t_envp **alst, t_envp *new)
{
	t_envp	*lst;

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
