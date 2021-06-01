#include "../../../includes/minishell.h"

t_slist	*ft_slstnew(t_parsed *content)
{
	t_slist	*elem;

	elem = (t_slist *)malloc(sizeof(t_slist));
	if (elem == NULL)
		return (NULL);
	elem->content = content;
	elem->next = NULL;
	elem->prev = NULL;
	return (elem);
}
