/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rlstdelone.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 16:48:31 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/26 16:59:30 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_rlstdelone(t_rlist *lst, void (*del)(t_redirect *))
{
	if (lst)
	{
		del(lst->content);
		free(lst);
	}
}
