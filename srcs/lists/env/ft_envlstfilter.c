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

t_envp	*ft_envlstfilter(t_envp *lst, t_bool (*f)(t_env *))
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
		lst = next;
	}
	prev_envlst(result);
	return (result);
}

t_envp	*ft_envlstfilterarg(t_envp *lst, t_bool (*f)(t_env *, char *arg),
	void (*del)(t_env *), char *arg)
{
	t_envp	*result;
	t_envp	*next;

	result = NULL;
	while (lst)
	{
		next = lst->next;
		if (f(lst->content, arg))
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

void	ft_envlstfilter_exec(t_shell *shell)
{
	t_envp	*tmp;
	int		i;
	char	*tmp2;
	char	*tmp3;

	ft_split_del(shell->envp);
	shell->envp = ft_calloc(ft_envlstsize(shell->envp_2) + 1, sizeof(char *));
	tmp = shell->envp_2;
	i = 0;
	while (tmp)
	{
		if (tmp->content->has_val)
		{
			tmp2 = ft_strjoin(tmp->content->key, "=");
			if (!tmp->content->has_val)
				tmp3 = ft_strjoin(tmp2, "");
			else
				tmp3 = ft_strjoin(tmp2, tmp->content->val);
			shell->envp[i] = ft_strdupdel(tmp3);
			ft_strdel(&tmp2);
			i++;
		}
		tmp = tmp->next;
	}
}
