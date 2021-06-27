#include "../../../includes/minishell.h"

t_pids	*ft_plstnew(pid_t *content)
{
	t_pids	*elem;

	elem = (t_pids *)malloc(sizeof(t_pids));
	if (elem == NULL)
		return (NULL);
	elem->content = content;
	elem->next = NULL;
	elem->prev = NULL;
	return (elem);
}
