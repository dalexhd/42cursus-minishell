/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_slstiter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 16:51:13 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/11 21:14:28 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_slstiter(t_slist *lst, void (*f)(t_parsed *))
{
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}
