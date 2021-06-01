#include "../../../includes/minishell.h"

void	ft_alstadd_front(t_alist **alst, t_alist *new)
{
	if (alst)
		new->next = *alst;
	*alst = new;
}
