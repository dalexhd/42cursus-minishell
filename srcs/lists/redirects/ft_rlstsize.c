#include "../../../includes/minishell.h"

int	ft_rlstsize(t_rlist *lst)
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
