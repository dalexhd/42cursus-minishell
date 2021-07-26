#include "../../includes/minishell.h"

static char	*ft_safesplit_init(int *flags, int *size, t_aslist **list, char *s)
{
	*flags = 0;
	*size = 0;
	*list = NULL;
	return (s);
}

static	void	sec(char *start, int size, char *s, t_aslist **list)
{
	t_asrg		*arg;
	char		*tmp;

	tmp = ft_calloc(sizeof(char), size + 1);
	ft_memcpy(tmp, start, size);
	arg = (t_asrg *)malloc(sizeof(t_asrg));
	arg->arg = tmp;
	arg->next = s[0];
	arg->concat = *list && ft_aslstlast(*list)
		&& ft_aslstlast(*list)->content->next != g_running;
	ft_aslstadd_back(&(*list), ft_aslstnew(arg));
}

static t_bool	soft_split(char **s, int (*b)[4], char **t)
{
	t_bool	split;

	split = false;
	if (**s)
	{
		(*b)[0] = ((*b)[1] != (*b)[3] && (*b)[2] > 1
				&& !(*b)[3]) || ((*b)[2] > 0 && !(*b)[3]
				&& ft_strchr(*t, (*s)[1]) && (*s)[0] != '\\');
		(*s)++;
		split = true;
	}
	return (split);
}

t_aslist	*ft_safesplitlist_new(char *s, char c, char *t)
{
	char		*z;
	t_aslist	*list;
	int			b[4];

	z = ft_safesplit_init(&b[3], &b[2], &list, s);
	while (1)
	{
		b[1] = b[3];
		if (ft_strchr(t, *s) && (!b[3] || b[3] & 1 << (ft_strchr(t, *s) - t)))
			b[3] ^= 1 << (ft_strchr(t, *s) - t);
		if (!*s || (*s == c && !b[3]) || b[0])
		{
			if (!*s)
				b[2] = ft_strlen(z);
			if (b[2] > 0)
				sec(z, b[2], s, &list);
			z += b[2] + (!b[0] || z[b[2]] == c);
			b[2] = s[0] != c;
		}
		else
			b[2]++;
		if (!soft_split(&s, &b, &t))
			break ;
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
				sec(start, size + (!l && ft_strchr(st, *s)), s, &list);
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
