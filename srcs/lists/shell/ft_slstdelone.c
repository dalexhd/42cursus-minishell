#include "../../../includes/minishell.h"

void	ft_slstdelone(t_slist *lst, void (*del)(t_parsed *))
{
	if (lst)
	{
		del(lst->content);
		free(lst);
	}
}
