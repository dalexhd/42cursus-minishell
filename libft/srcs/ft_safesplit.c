/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safesplit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/24 10:05:54 by aborboll          #+#    #+#             */
/*   Updated: 2021/07/01 21:41:54 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static void	ft_safesplit_init(int *flags, int *size, t_list **list)
{
	*flags = 0;
	*size = 0;
	*list = NULL;
}

t_list	*ft_safesplitlist(char *s, char c, char *set)
{
	int		size;
	int		fl;
	char	*start;
	t_list	*list;
	char	*tmp;

	ft_safesplit_init(&fl, &size, &list);
	start = s;
	while (1)
	{
		if (ft_strchr(set, *s) && (!fl || fl & 1 << (ft_strchr(set, *s) - set)))
			fl ^= 1 << (ft_strchr(set, *s) - set);
		if (!*s || (*s == c && !fl))
		{
			if (size > 0)
			{
				tmp = ft_strcut(start, 0, size);
				ft_lstadd_back(&list, ft_lstnew(tmp));
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
