/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_slstdelone.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 16:48:31 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/11 21:14:24 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_slstdelone(t_slist *lst, void (*del)(t_parsed *))
{
	if (lst)
	{
		del(lst->content);
		free(lst);
	}
}
