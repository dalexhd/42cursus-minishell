#include "../../../includes/minishell.h"

void	ft_alstiter(t_alist *lst, void (*f)(t_args *))
{
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}
