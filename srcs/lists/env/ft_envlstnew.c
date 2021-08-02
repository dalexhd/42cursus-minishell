#include "../../../includes/minishell.h"

t_envp	*ft_envlstnew(t_env *content)
{
	t_envp	*elem;

	elem = (t_envp *)malloc(sizeof(t_envp));
	if (elem == NULL)
		return (NULL);
	elem->content = content;
	elem->next = NULL;
	elem->prev = NULL;
	return (elem);
}
