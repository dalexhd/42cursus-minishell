#include "../../../includes/minishell.h"

void	ft_aslstdelone(t_aslist *lst, void (*del)(t_asrg *))
{
	if (lst)
	{
		del(lst->content);
		free(lst);
	}
}
