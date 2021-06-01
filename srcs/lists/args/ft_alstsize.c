#include "../../../includes/minishell.h"

int	ft_alstsize(t_alist *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
