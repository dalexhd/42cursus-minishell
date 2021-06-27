#include "../../../includes/minishell.h"

void	ft_plstadd_front(t_pids **alst, t_pids *new)
{
	if (*alst)
	{
		new->next = *alst;
		(*alst)->prev = new;
	}
	*alst = new;
}
