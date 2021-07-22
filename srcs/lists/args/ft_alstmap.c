#include "../../../includes/minishell.h"

static t_alist	*ret_map(t_alist *lst, void (*del)(t_args *))
{
	del(lst->content);
	free(lst);
	return (NULL);
}

t_alist	*ft_alstmap(t_alist *lst, void *(*f)(t_args *), void (*del)(t_args *))
{
	t_alist		*new;
	t_alist		*first;

	if (!(f == NULL || lst == NULL))
	{
		new = ft_alstnew(f(lst->content));
		if (new == NULL)
			ret_map(lst, del);
		else
		{
			first = new;
			while (lst->next != NULL)
			{
				lst = lst->next;
				new->next = ft_alstnew(f(lst->content));
				if (new->next == NULL)
					ret_map(lst, del);
				new = new->next;
			}
			return (first);
		}
	}
	return (NULL);
}
