#include "../../../includes/minishell.h"

void	ft_hlstadd_front(t_hist **alst, t_hist *new)
{
	if (*alst)
	{
		new->next = *alst;
		(*alst)->prev = new;
	}
	*alst = new;
}
