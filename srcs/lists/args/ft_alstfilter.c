#include "../../../includes/minishell.h"

static	t_alist	*ft_alstpush(t_alist **lst, t_alist *sec)
{
	t_alist	*current;

	if (!lst)
		return (NULL);
	if (!*lst)
		return (*lst = sec);
	current = *lst;
	if (!(current))
	{
		*lst = sec;
		return (*lst);
	}
	while (current->next)
		current = current->next;
	current->next = sec;
	return (*lst);
}

static	void	prev_alst(t_alist *lst)
{
	if (lst)
	{
		if (lst->next)
			lst->next->prev = lst;
		prev_alst(lst->next);
	}
}

t_alist	*ft_alstfilter(t_alist *lst, t_bool (*f)(t_args *),
	void (*del)(t_args *))
{
	t_alist	*result;
	t_alist	*next;

	result = NULL;
	while (lst)
	{
		next = lst->next;
		if (f(lst->content))
		{
			lst->next = NULL;
			ft_alstpush(&result, lst);
		}
		else
			ft_alstdelone(lst, del);
		lst = next;
	}
	prev_alst(result);
	return (result);
}
