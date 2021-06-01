#include "../../../includes/minishell.h"

t_alist	*ft_alstmap(t_alist *lst, void *(*f)(void *), void (*del)(void *))
{
	t_alist		*new;
	t_alist		*first;

	(void)(*del);
	if (f == NULL || lst == NULL)
		return (NULL);
	new = ft_alstnew(f(lst->content));
	if (new == NULL)
		return (NULL);
	first = new;
	while (lst->next != NULL)
	{
		lst = lst->next;
		new->next = ft_alstnew(f(lst->content));
		if (new->next == NULL)
			return (NULL);
		new = new->next;
	}
	return (first);
}
