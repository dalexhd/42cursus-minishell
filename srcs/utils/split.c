#include "../../includes/minishell.h"

t_aslist		*ft_safesplitlist_new(char *s, char c, char *set, t_bool force_set)
{
	int			size;
	int			fl;
	char		*start;
	t_aslist	*list;
	t_asrg		*arg;
	char		*tmp;
	int			bef_bool;
	int			bef_bool2;

	(void)force_set;
	fl = 0;
	size = 0;
	list = NULL;
	start = s;
	bef_bool2 = 0;
	while (1)
	{
		bef_bool = fl;
		if (ft_strchr(set, *s) && (!fl || fl & 1 << (ft_strchr(set, *s) - set)))
			fl ^= 1 << (ft_strchr(set, *s) - set);
		if (!*s || (*s == c && !fl) || bef_bool2)
		{
			if (!*s)
				size = ft_strlen(start);
			if (size > 0)
			{
				tmp = ft_calloc(sizeof(char), size + 1);
				ft_memcpy(tmp, start, size);
				arg = (t_asrg *)malloc(sizeof(t_asrg));
				arg->arg = tmp;
				arg->next = s[0];
				arg->concat = list && ft_aslstlast(list) && ft_aslstlast(list)->content->next != c;
				ft_aslstadd_back(&list, ft_aslstnew(arg));
			}
			start += size + (!bef_bool2 || start[size] == c);
			size = s[0] != c;
		}
		else
			size++;
		if (!*s)
			break ;
		if ((bef_bool != fl && size > 1 && !fl) || (size > 0 && !fl && ft_strchr(set, s[1]) && s[0] != '\\'))
			bef_bool2 = 1;
		else
			bef_bool2 = 0;
		s++;
	}
	return (list);
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
