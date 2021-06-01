/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_alstiter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 16:51:13 by aborboll          #+#    #+#             */
/*   Updated: 2021/06/01 17:41:35 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_alstiter(t_alist *lst, void (*f)(t_args *))
{
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}
