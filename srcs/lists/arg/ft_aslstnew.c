#include "../../../includes/minishell.h"

t_aslist	*ft_aslstnew(t_asrg *content)
{
	t_aslist	*elem;

	elem = (t_aslist *)malloc(sizeof(t_aslist));
	if (elem == NULL)
		return (NULL);
	elem->content = content;
	elem->next = NULL;
	elem->prev = NULL;
	return (elem);
}
