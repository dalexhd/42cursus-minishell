#include "../../../includes/minishell.h"

void	ft_slstadd_front(t_slist **alst, t_slist *new)
{
	if (alst)
		new->next = *alst;
	*alst = new;
}
