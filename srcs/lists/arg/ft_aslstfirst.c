#include "../../../includes/minishell.h"

t_aslist	*ft_aslstfirst(t_aslist *lst)
{
	if (!lst)
		return (0);
	while (lst->prev != 0)
	{
		lst = lst->prev;
	}
	return (lst);
}
