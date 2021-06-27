#include "../../../includes/minishell.h"

int	ft_plstsize(t_pids *lst)
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
