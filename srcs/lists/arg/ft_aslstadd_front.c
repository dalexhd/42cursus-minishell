#include "../../../includes/minishell.h"

void	ft_aslstadd_front(t_aslist **alst, t_aslist *new)
{
	if (alst)
		new->next = *alst;
	*alst = new;
}
