/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcut.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/24 23:10:31 by aborboll          #+#    #+#             */
/*   Updated: 2021/06/07 20:23:04 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strcut(char *str, int from, int to)
{
	char	*begin;

	if (from >= to)
		return (ft_strdup(" "));
	begin = ft_calloc(sizeof(char), (to - from) + 1);
	ft_memcpy(begin, str + from, to);
	return (begin);
}
