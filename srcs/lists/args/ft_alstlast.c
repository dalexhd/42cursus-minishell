#include "../../../includes/minishell.h"

t_alist	*ft_alstlast(t_alist *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}
