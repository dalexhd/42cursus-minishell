#include "../../../includes/minishell.h"

void	ft_aslstclear(t_aslist **lst, void (*del)(void *))
{
	t_aslist	*next;
	t_aslist	*new;

	new = *lst;
	if (new)
	{
		while (new)
		{
			next = new->next;
			del(new->content->arg);
			free(new->content);
			free(new);
			new = next;
		}
		*lst = NULL;
	}
}
