#include "../../../includes/minishell.h"

void	ft_plstdelone(t_pids *lst, void (*del)(void*))
{
	if (lst)
	{
		del(lst->content);
		free(lst);
	}
}
