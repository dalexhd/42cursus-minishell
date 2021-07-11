#include "../../includes/minishell.h"

t_bool	arg_has_red(t_alist *arg)
{
	t_bool	status;

	status = false;
	if (arg->content
		&& (arg->content->type == R_IN
			|| arg->content->type == R_OUT
			|| arg->content->type == R_AOUT))
	{
		status = true;
	}
	return (status);
}

int	get_quote_type(char *cmd)
{
	size_t	len;

	len = ft_strlen(cmd);
	if (cmd[0] == '"' && cmd[len - 1] == '"')
		return (D_QUOT);
	else if (cmd[0] == '\'' && cmd[len - 1] == '\'')
		return (S_QUOT);
	else
		return (N_QUOT);
}


static	int	filtering_args(t_alist **tmp, t_alist *args)
{
	if (!args)
		return (0);
	else if (args->content->readable)
		ft_alstadd_back(&(*tmp), ft_alstnew(args->content));
	return (filtering_args(tmp, args->next));
}

void	filter_readable_args(t_slist **parsed)
{
	t_alist	*tmp;

	if (*parsed)
	{
		tmp = (t_alist *)malloc(sizeof(t_alist));
		tmp = NULL;
		while (1)
		{
			if (!filtering_args(&tmp, (*parsed)->content->args))
				break ;
		}
		(*parsed)->content->args = tmp;
		filter_readable_args(&(*parsed)->next);
	}
}

static void	ft_safesplit_init(int *flags, int *size, t_aslist **list)
{
	*flags = 0;
	*size = 0;
	*list = NULL;
}

t_aslist		*ft_safesplitlist(char *s, char c, char *set, t_bool force_set)
{
	int			size;
	int			fl;
	char		*start;
	t_aslist	*list;
	t_asrg		*arg;
	char		*tmp;

	ft_safesplit_init(&fl, &size, &list);
	start = s;
	while (1)
	{
		if (ft_strchr(set, *s) && (!fl || fl & 1 << (ft_strchr(set, *s) - set)))
			fl ^= 1 << (ft_strchr(set, *s) - set);
		if ((!*s || (((force_set && ft_strchr(set, *s) && s[1] != c) || *s == c) && !fl))
			|| (!fl && force_set && ft_strchr(set, *s) && s[1] != c))
		{
			if (size > 0)
			{
				tmp = ft_calloc(sizeof(char), size + (!fl && ft_strchr(set, *s)) + 1);
				ft_memcpy(tmp, start, size + (!fl && ft_strchr(set, *s)));
				arg = (t_asrg *)malloc(sizeof(t_asrg));
				arg->arg = tmp;
				arg->next = s[0];
				arg->concat = list && ft_aslstlast(list) && ft_aslstlast(list)->content->next != c;
				ft_aslstadd_back(&list, ft_aslstnew(arg));
			}
			start += size + 1;
			size = 0;
		}
		else
			size++;
		if (!*s)
			break ;
		s++;
	}
	return (list);
}

/* t_aslist		*ft_safesplitlist(char *s, char c, char *set, t_bool force_set)
{
	int			size;
	int			fl;
	char		*start;
	t_aslist	*list;
	t_asrg		*arg;
	char		*tmp2;
	char		*tmp;

	ft_safesplit_init(&fl, &size, &list);
	start = s;
	while (1)
	{
		if (ft_strchr(set, *s) && (!fl || fl & 1 << (ft_strchr(set, *s) - set)))
			fl ^= 1 << (ft_strchr(set, *s) - set);
		if ((!*s || (((force_set && ft_strchr(set, *s) && s[1] != c) || *s == c) && !fl))
			|| (fl && force_set && ft_strchr(set, *s) && s[1] != c))
		{
			if (size > 0)
			{
				tmp = ft_calloc(sizeof(char), size + (!fl && ft_strchr(set, *s)) + 1);
				ft_memcpy(tmp, start, size + (!fl && ft_strchr(set, *s)));
				arg = (t_asrg *)malloc(sizeof(t_asrg));
				tmp2 = ft_strchr(tmp, *s) ? &ft_strchr(tmp, *s)[0] : "";
				arg->arg = ft_strjoin(tmp2, tmp);
				arg->next = s[0];
				arg->concat = list && ft_aslstlast(list) && ft_aslstlast(list)->content->next != c;
				ft_aslstadd_back(&list, ft_aslstnew(arg));
			}
			start += size + 1;
			size = 0;
		}
		else
			size++;
		if (!*s)
			break ;
		s++;
	}
	return (list);
} */
