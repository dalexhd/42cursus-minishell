/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstfirst_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 16:35:49 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/20 16:37:21 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

t_list	*ft_lstfirst(t_list *lst)
{
	if (!lst)
		return (0);
	while (lst->prev != 0)
	{
		lst = lst->prev;
	}
	return (lst);
}
