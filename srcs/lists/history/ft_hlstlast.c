#include "../../../includes/minishell.h"

t_hist	*ft_hlstlast(t_hist *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}
