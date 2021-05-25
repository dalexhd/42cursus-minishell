/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcut.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/24 23:10:31 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/24 23:17:03 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strcut(char *str, int from, int to)
{
	char	*cut;
	char	*begin;

	if (from >= to)
		return (NULL);
	cut = ft_calloc(sizeof(char), (to - from) + 1);
	begin = cut;
	if (!cut)
		return (NULL);
	ft_memcpy(cut, str + from, to);
	return (begin);
}
