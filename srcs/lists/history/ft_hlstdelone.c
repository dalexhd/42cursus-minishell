/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hlstdelone.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 16:48:31 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/26 21:13:50 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_hlstdelone(t_hist *lst, void (*del)(void*))
{
	if (lst)
	{
		del(lst->copy);
		del(lst->original);
		free(lst);
	}
}
