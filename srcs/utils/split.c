#include "../../includes/minishell.h"

static char	*ft_safesplit_init(int *flags, int *size, t_aslist **list, char *s)
{
	*flags = 0;
	*size = 0;
	*list = NULL;
	return (s);
}

static	void	sec(char *start, int size, char *s, t_aslist **list, char c)
{
	t_asrg		*arg;
	char		*tmp;

	tmp = ft_calloc(sizeof(char), size + 1);
	ft_memcpy(tmp, start, size);
	arg = (t_asrg *)malloc(sizeof(t_asrg));
	arg->arg = tmp;
	arg->next = s[0];
	arg->concat = *list && ft_aslstlast(*list)
		&& ft_aslstlast(*list)->content->next != c;
	ft_aslstadd_back(&(*list), ft_aslstnew(arg));
}

t_aslist		*ft_safesplitlist_new(char *s, char c, char *set)
{
	int			size;
	int			fl;
	char		*start;
	t_aslist	*list;
	int			bef[2];

	start = ft_safesplit_init(&fl, &size, &list, s);
	while (1)
	{
		bef[1] = fl;
		if (ft_strchr(set, *s) && (!fl || fl & 1 << (ft_strchr(set, *s) - set)))
			fl ^= 1 << (ft_strchr(set, *s) - set);
		if (!*s || (*s == c && !fl) || bef[0])
		{
			if (!*s)
				size = ft_strlen(start);
			if (size > 0)
				sec(start, size, s, &list, c);
			start += size + (!bef[0] || start[size] == c);
			size = s[0] != c;
		}
		else
			size++;
		if (!*s)
			break ;
		bef[0] = (bef[1] != fl && size > 1 && !fl) || (size > 0 && !fl
				&& ft_strchr(set, s[1]) && s[0] != '\\');
		s++;
	}
	return (list);
}

t_aslist	*ft_safesplitlist(char *s, char c, char *st)
{
	int			size;
	int			l;
	char		*start;
	t_aslist	*list;

	start = ft_safesplit_init(&l, &size, &list, s);
	while (1)
	{
		if (ft_strchr(st, *s) && (!l || l & 1 << (ft_strchr(st, *s) - st)))
			l ^= 1 << (ft_strchr(st, *s) - st);
		if ((!*s || ((*s == c) && !l)))
		{
			if (size > 0)
				sec(start, size + (!l && ft_strchr(st, *s)), s, &list, c);
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
