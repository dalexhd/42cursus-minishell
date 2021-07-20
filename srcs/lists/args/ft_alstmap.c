#include "../../../includes/minishell.h"

t_alist	*ft_alstmap(t_alist *lst, void *(*f)(t_args *), void (*del)(t_args *))
{
	t_alist		*new;
	t_alist		*first;

	if (f == NULL || lst == NULL)
		return (NULL);
	new = ft_alstnew(f(lst->content));
	if (new == NULL)
	{
		del(lst->content);
		free(lst);
		return (NULL);
	}
	first = new;
	while (lst->next != NULL)
	{
		lst = lst->next;
		new->next = ft_alstnew(f(lst->content));
		if (new->next == NULL)
		{
			del(lst->content);
			free(lst);
			return (NULL);
		}
		new = new->next;
	}
	return (first);
}
