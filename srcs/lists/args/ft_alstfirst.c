/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_alstfirst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 21:06:18 by aborboll          #+#    #+#             */
/*   Updated: 2021/06/01 17:46:27 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_alist	*ft_alstfirst(t_alist *lst)
{
	if (!lst)
		return (0);
	while (lst->prev != 0)
	{
		lst = lst->prev;
	}
	return (lst);
}
