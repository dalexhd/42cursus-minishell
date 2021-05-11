/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_slstmap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 18:40:01 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/11 17:45:43 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_slist	*ft_slstmap(t_slist *lst, void *(*f)(void *), void (*del)(void *))
{
	t_slist		*new;
	t_slist		*first;

	(void)(*del);
	if (f == NULL || lst == NULL)
		return (NULL);
	new = ft_slstnew(f(lst->content));
	if (new == NULL)
		return (NULL);
	first = new;
	while (lst->next != NULL)
	{
		lst = lst->next;
		new->next = ft_slstnew(f(lst->content));
		if (new->next == NULL)
			return (NULL);
		new = new->next;
	}
	return (first);
}
