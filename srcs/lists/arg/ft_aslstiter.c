#include "../../../includes/minishell.h"

void	ft_aslstiter(t_aslist *lst, void (*f)(t_asrg *))
{
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}
