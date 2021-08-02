#include "../../../includes/minishell.h"

void	ft_envlstiter(t_envp *lst, void (*f)(t_env *))
{
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}
