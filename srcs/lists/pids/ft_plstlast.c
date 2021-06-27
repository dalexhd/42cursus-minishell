#include "../../../includes/minishell.h"

t_pids	*ft_plstlast(t_pids *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}
