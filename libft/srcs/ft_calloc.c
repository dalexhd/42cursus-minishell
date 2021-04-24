/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 13:41:26 by aborboll          #+#    #+#             */
/*   Updated: 2021/04/15 18:42:15 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	t_uchar	*tab;
	size_t	i;

	i = 0;
	tab = (t_uchar *)malloc(size * count);
	if (!tab)
		return (NULL);
	while (i < count * size)
		tab[i++] = '\0';
	return (tab);
}
