#include "../../../includes/minishell.h"

void	ft_slstiter(t_slist *lst, void (*f)(t_parsed *))
{
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}
