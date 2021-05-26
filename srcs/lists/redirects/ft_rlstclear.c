/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rlstclear.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 15:49:43 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/26 16:59:30 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_rlstclear(t_rlist **lst, void (*del)(t_redirect *))
{
	t_rlist	*next;
	t_rlist	*new;

	new = *lst;
	if (new)
	{
		while (new)
		{
			next = new->next;
			del(new->content);
			free(new);
			new = next;
		}
		*lst = NULL;
	}
}
