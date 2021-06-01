#include "../../../includes/minishell.h"

int	ft_hlstsize(t_hist *lst)
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
