#include "../../../includes/minishell.h"

void	ft_hlstclear(t_hist **lst, void (*del)(void*))
{
	t_hist	*next;
	t_hist	*new;

	new = *lst;
	if (new)
	{
		while (new)
		{
			next = new->next;
			del(new->original);
			del(new->copy);
			free(new);
			new = next;
		}
		*lst = NULL;
	}
}
