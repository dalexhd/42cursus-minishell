/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rlstiter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 16:51:13 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/26 16:59:30 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_rlstiter(t_rlist *lst, void (*f)(t_redirect *))
{
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}
