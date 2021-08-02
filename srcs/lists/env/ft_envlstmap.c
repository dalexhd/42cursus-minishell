#include "../../../includes/minishell.h"

static t_envp	*ret_map(t_envp *lst, void (*del)(t_env *))
{
	del(lst->content);
	free(lst);
	return (NULL);
}

t_envp	*ft_envlstmap(t_envp *lst, void *(*f)(t_env *), void (*del)(t_env *))
{
	t_envp		*new;
	t_envp		*first;

	if (!(f == NULL || lst == NULL))
	{
		new = ft_envlstnew(f(lst->content));
		if (new == NULL)
			ret_map(lst, del);
		else
		{
			first = new;
			while (lst->next != NULL)
			{
				lst = lst->next;
				new->next = ft_envlstnew(f(lst->content));
				if (new->next == NULL)
					ret_map(lst, del);
				new = new->next;
			}
			return (first);
		}
	}
	return (NULL);
}
