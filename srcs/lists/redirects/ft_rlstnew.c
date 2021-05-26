/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rlstnew.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 16:36:06 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/26 16:59:30 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_rlist	*ft_rlstnew(t_redirect *content)
{
	t_rlist	*elem;

	elem = (t_rlist *)malloc(sizeof(t_rlist));
	if (elem == NULL)
		return (NULL);
	elem->content = content;
	elem->next = NULL;
	elem->prev = NULL;
	return (elem);
}
