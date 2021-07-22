#include "../../../includes/minishell.h"

void	ft_alstdelone(t_alist *lst, void (*del)(t_args *))
{
	if (lst)
	{
		del(lst->content);
		free(lst);
		lst = NULL;
	}
}
