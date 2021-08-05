#include "../../../includes/minishell.h"

void	ft_envlstadd_back(t_envp **envlst, t_envp *new)
{
	t_envp	*lst;

	if (envlst && new)
	{
		if (!(*envlst))
			*envlst = new;
		else
		{
			lst = *envlst;
			while (lst->next)
				lst = lst->next;
			lst->next = new;
			new->prev = lst;
		}
	}
}
