#include "../../../includes/minishell.h"

t_rlist	*ft_rlstnew(t_redirect *content)
{
	t_rlist	*elem;

	elem = (t_rlist *)malloc(sizeof(t_rlist));
	if (elem == NULL)
		return (NULL);
	elem->content = content;
	elem->next = NULL;
	elem->prev = NULL;
	return (elem);
}
