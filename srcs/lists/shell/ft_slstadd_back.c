/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_slstadd_back.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 16:46:21 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/26 16:59:47 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_slstadd_back(t_slist **alst, t_slist *new)
{
	t_slist	*lst;

	if (alst && new)
	{
		if (!(*alst))
			*alst = new;
		else
		{
			lst = *alst;
			while (lst->next)
				lst = lst->next;
			lst->next = new;
			new->prev = lst;
		}
	}
}
