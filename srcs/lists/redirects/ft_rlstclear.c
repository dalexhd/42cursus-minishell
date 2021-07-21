#include "../../../includes/minishell.h"

void	ft_rlstclear(t_rlist **lst, void (*del)(void *))
{
	t_rlist	*next;
	t_rlist	*new;

	new = *lst;
	if (new)
	{
		while (new)
		{
			next = new->next;
			if (new->content->in.status)
				del(new->content->in.file);
			if (new->content->out.status)
				del(new->content->out.file);
			if (new->content->aout.status)
				del(new->content->aout.file);
			del(new->content);
			free(new);
			new = next;
		}
		*lst = NULL;
	}
}
