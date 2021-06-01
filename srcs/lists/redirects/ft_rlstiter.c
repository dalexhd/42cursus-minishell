#include "../../../includes/minishell.h"

void	ft_rlstiter(t_rlist *lst, void (*f)(t_redirect *))
{
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}
