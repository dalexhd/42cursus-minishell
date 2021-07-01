#include "../../../includes/minishell.h"

void	ft_alstclear(t_alist **lst, void (*del)(t_args *))
{
	t_alist	*next;
	t_alist	*new;

	new = *lst;
	if (new)
	{
		while (new)
		{
			next = new->next;
			del(new->content);
			free(new);
			new = next;
		}
		*lst = NULL;
	}
}
