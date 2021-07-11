#include "../../../includes/minishell.h"

t_aslist	*ft_aslstlast(t_aslist *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}
