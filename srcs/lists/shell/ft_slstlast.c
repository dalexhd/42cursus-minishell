#include "../../../includes/minishell.h"

t_slist	*ft_slstlast(t_slist *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}
