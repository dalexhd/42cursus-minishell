#include "../../../includes/minishell.h"

int	ft_aslstsize(t_aslist *lst)
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
