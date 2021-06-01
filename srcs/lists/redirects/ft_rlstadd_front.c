#include "../../../includes/minishell.h"

void	ft_rlstadd_front(t_rlist **alst, t_rlist *new)
{
	if (alst)
		new->next = *alst;
	*alst = new;
}
