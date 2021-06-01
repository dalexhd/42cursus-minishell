#include "../../../includes/minishell.h"

t_rlist	*ft_rlstlast(t_rlist *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}
