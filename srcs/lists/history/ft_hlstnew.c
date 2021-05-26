/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hlstnew.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 16:36:06 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/26 21:02:43 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_hist	*ft_hlstnew(void *content)
{
	t_hist	*elem;

	elem = (t_hist *)malloc(sizeof(t_hist));
	if (elem == NULL)
		return (NULL);
	elem->original = ft_strdup(content);
	elem->copy = ft_strdup(content);
	elem->next = NULL;
	elem->prev = NULL;
	return (elem);
}
