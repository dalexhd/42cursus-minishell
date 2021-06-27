#include "../../../includes/minishell.h"

void	ft_plstclear(t_pids **lst, void (*del)(void*))
{
	t_pids	*next;
	t_pids	*new;

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
