#include "../../../includes/minishell.h"

t_alist	*ft_alstfirst(t_alist *lst)
{
	if (!lst)
		return (0);
	while (lst->prev != 0)
	{
		lst = lst->prev;
	}
	return (lst);
}
