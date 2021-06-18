/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strduplen.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 17:22:55 by aborboll          #+#    #+#             */
/*   Updated: 2021/06/18 17:30:31 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strduplen(const char *s, size_t len)
{
	char	*dst;

	len = len + 1;
	dst = (char *) malloc(sizeof(char) * len);
	if (dst == NULL)
		return (NULL);
	ft_memcpy(dst, s, len);
	return (dst);
}
