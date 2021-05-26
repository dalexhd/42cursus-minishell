/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rlstadd_front.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 16:41:29 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/26 16:58:59 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_rlstadd_front(t_rlist **alst, t_rlist *new)
{
	if (alst)
		new->next = *alst;
	*alst = new;
}
