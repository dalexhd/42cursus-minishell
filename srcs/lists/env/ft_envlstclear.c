#include "../../../includes/minishell.h"

void	ft_envlstclear(t_envp **lst, void (*del)(t_env *))
{
	t_envp	*next;
	t_envp	*new;

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
