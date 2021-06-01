#include "../../../includes/minishell.h"

t_alist	*ft_alstnew(t_args *content)
{
	t_alist	*elem;

	elem = (t_alist *)malloc(sizeof(t_alist));
	if (elem == NULL)
		return (NULL);
	elem->content = content;
	elem->next = NULL;
	elem->prev = NULL;
	return (elem);
}
