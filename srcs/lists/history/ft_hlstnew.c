#include "../../../includes/minishell.h"

t_hist	*ft_hlstnew(void *content)
{
	t_hist	*elem;

	elem = (t_hist *)malloc(sizeof(t_hist));
	if (elem == NULL)
		return (NULL);
	elem->original = ft_strdup(content);
	elem->copy = ft_strdup(content);
	elem->next = NULL;
	elem->prev = NULL;
	return (elem);
}
