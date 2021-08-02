#include "../../../includes/minishell.h"

t_envp	*ft_envlstlast(t_envp *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}
