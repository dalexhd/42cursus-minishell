/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 16:35:18 by aborboll          #+#    #+#             */
/*   Updated: 2021/08/09 11:24:55 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;
	t_uchar	*s;
	t_uchar	*d;

	if (dst == NULL && src == NULL)
		return (0);
	d = (t_uchar *)dst;
	s = (t_uchar *)src;
	i = 0;
	if (!n || dst == src)
		return (dst);
	while (n--)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}
