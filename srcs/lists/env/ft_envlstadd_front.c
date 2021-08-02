#include "../../../includes/minishell.h"

void	ft_envlstadd_front(t_envp **alst, t_envp *new)
{
	if (alst)
		new->next = *alst;
	*alst = new;
}
