#include "../../../includes/minishell.h"

void	ft_rlstdelone(t_rlist *lst, void (*del)(t_redirect *))
{
	if (lst)
	{
		del(lst->content);
		free(lst);
	}
}
