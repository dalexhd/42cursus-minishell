/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hlstclear.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 15:49:43 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/26 21:14:11 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_hlstclear(t_hist **lst, void (*del)(void*))
{
	t_hist	*next;
	t_hist	*new;

	new = *lst;
	if (new)
	{
		while (new)
		{
			next = new->next;
			del(new->original);
			del(new->copy);
			free(new);
			new = next;
		}
		*lst = NULL;
	}
}
