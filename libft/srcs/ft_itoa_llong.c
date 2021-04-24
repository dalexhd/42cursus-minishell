/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_llong.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 13:41:44 by aborboll          #+#    #+#             */
/*   Updated: 2021/04/15 19:00:55 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_itoa_llong(t_llong n)
{
	size_t	i;
	char	*ret;
	int		sign;

	i = 0;
	ret = ft_strnew(ft_numlen(n));
	if (!ret)
		return (NULL);
	if (n == 0)
		return (ft_strdup("0"));
	sign = n < 0;
	while (n != 0)
	{
		ret[i++] = ft_abs(n % 10) + '0';
		n /= 10;
	}
	if (sign)
		ret[i++] = '-';
	ret[i] = '\0';
	return (ft_strrev(ret));
}
