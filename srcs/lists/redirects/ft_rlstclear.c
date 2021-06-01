#include "../../../includes/minishell.h"

void	ft_rlstclear(t_rlist **lst, void (*del)(t_redirect *))
{
	t_rlist	*next;
	t_rlist	*new;

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
