#include "../../../includes/minishell.h"

void	ft_hlstdelone(t_hist *lst, void (*del)(void*))
{
	if (lst)
	{
		del(lst->copy);
		del(lst->original);
		free(lst);
	}
}
