#include "../../../includes/minishell.h"

int	ft_slstsize(t_slist *lst)
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
