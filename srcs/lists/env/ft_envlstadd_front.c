#include "../../../includes/minishell.h"

void	ft_envlstadd_front(t_envp **envlst, t_envp *new)
{
	if (envlst)
		new->next = *envlst;
	*envlst = new;
}
