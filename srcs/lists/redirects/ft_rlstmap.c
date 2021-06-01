#include "../../../includes/minishell.h"

t_rlist	*ft_rlstmap(t_rlist *lst, void *(*f)(void *), void (*del)(void *))
{
	t_rlist		*new;
	t_rlist		*first;

	(void)(*del);
	if (f == NULL || lst == NULL)
		return (NULL);
	new = ft_rlstnew(f(lst->content));
	if (new == NULL)
		return (NULL);
	first = new;
	while (lst->next != NULL)
	{
		lst = lst->next;
		new->next = ft_rlstnew(f(lst->content));
		if (new->next == NULL)
			return (NULL);
		new = new->next;
	}
	return (first);
}
