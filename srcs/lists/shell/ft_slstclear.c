#include "../../../includes/minishell.h"

void	ft_slstclear(t_slist **lst, void (*del)(t_parsed *))
{
	t_slist	*next;
	t_slist	*new;

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
