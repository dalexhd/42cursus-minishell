#include "../../../includes/minishell.h"

void	ft_alstclear(t_alist **lst)
{
	t_alist	*temp;

	if (*lst)
	{
		while ((*lst)->next != 0)
		{
			temp = *lst;
			*lst = (*lst)->next;
			free(temp);
		}
		free(*lst);
		*lst = NULL;
	}
}
