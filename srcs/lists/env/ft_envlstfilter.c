#include "../../../includes/minishell.h"

static	t_envp	*ft_envlstpush(t_envp **lst, t_envp *sec)
{
	t_envp	*current;

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

static	void	prev_envlst(t_envp *lst)
{
	if (lst)
	{
		if (lst->next)
			lst->next->prev = lst;
		prev_envlst(lst->next);
	}
}

t_envp	*ft_envlstfilter(t_envp *lst, t_bool (*f)(t_env *),
	void (*del)(t_env *))
{
	t_envp	*result;
	t_envp	*next;

	result = NULL;
	while (lst)
	{
		next = lst->next;
		if (f(lst->content))
		{
			lst->next = NULL;
			ft_envlstpush(&result, lst);
		}
		else
			ft_envlstdelone(lst, del);
		lst = next;
	}
	prev_envlst(result);
	return (result);
}
