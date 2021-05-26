/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hlstadd_back.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 16:46:21 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/26 20:58:06 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_hlstadd_back(t_hist **alst, t_hist *new)
{
	t_hist	*lst;

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
