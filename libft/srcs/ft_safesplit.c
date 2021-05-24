/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safesplit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/24 10:05:54 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/24 20:48:06 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"
#include <string.h>

t_list	*ft_safesplitlist(char *s, char c, char *set)
{
	int		size;
	int		flags;
	char	*start;
	char	*pos;
	t_list	*list;

	flags = 0;
	size = 0;
	start = s;
	list = NULL;
	while (1)
	{
		pos = ft_strchr(set, *s);
		if (pos && (!flags || flags & 1 << (pos - set)))
			flags ^= 1 << (pos - set);
		if (!*s || (*s == c && !flags))
		{
			if (size > 0)
			{
				const char *c = start;
				char *start = &c[0];
				char *end = &c[size];
				char *substr = (char *)calloc(1, end - start + 1);
				ft_memcpy(substr, start, end - start);
				ft_lstadd_back(&list, ft_lstnew(substr));
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

char	**ft_safesplit(char const *s, char c, char *set)
{
	int		size;
	int		i;
	char	**tokens;
	t_list	*list;

	list = ft_safesplitlist(s, c, set);
	size = ft_lstsize(list);
	tokens = (char **)malloc(sizeof(char*) * (size + 1));
	i = 0;
	while (list)
	{
		tokens[i] = ft_strtrim(list->content, "\"'");
		list = list->next;
		i++;
	}
	tokens[i] = NULL;
	return (tokens);
}
