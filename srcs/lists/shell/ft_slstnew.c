#include "../../../includes/minishell.h"

t_slist	*ft_slstnew(t_parsed *content)
{
	t_slist	*elem;

	(void)content;
	elem = (t_slist *)malloc(sizeof(t_slist));
	if (elem == NULL)
		return (NULL);
	elem->content = content;
	elem->content->valid = true;
	elem->content->line = NULL;
	elem->content->redirects = NULL;
	elem->next = NULL;
	elem->prev = NULL;
	return (elem);
}
