/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safesplit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/24 10:05:54 by aborboll          #+#    #+#             */
/*   Updated: 2021/06/03 21:07:00 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

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
				ft_lstadd_back(&list, ft_lstnew(ft_strcut(start, 0, size)));
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
