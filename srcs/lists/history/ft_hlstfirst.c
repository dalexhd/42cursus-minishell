/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hlstfirst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 21:06:18 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/26 21:06:44 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_hist	*ft_hlstfirst(t_hist *hst)
{
	if (!hst)
		return (0);
	while (hst->prev != 0)
	{
		hst = hst->prev;
	}
	return (hst);
}
