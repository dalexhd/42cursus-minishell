#include "../../../includes/minishell.h"

void	ft_envlstdelone(t_envp *lst, void (*del)(t_env *))
{
	if (lst)
	{
		del(lst->content);
		free(lst);
		lst = NULL;
	}
}
