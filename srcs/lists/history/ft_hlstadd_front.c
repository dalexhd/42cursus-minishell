/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hlstadd_front.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 16:41:29 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/26 21:43:26 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_hlstadd_front(t_hist **alst, t_hist *new)
{
	if (*alst)
	{
		new->next = *alst;
		(*alst)->prev = new;
	}
	*alst = new;
}
