#include "../../../includes/minishell.h"

t_envp	*ft_envlstfirst(t_envp *lst)
{
	if (!lst)
		return (0);
	while (lst->prev != 0)
	{
		lst = lst->prev;
	}
	return (lst);
}
