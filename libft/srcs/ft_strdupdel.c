/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdupdel.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 13:42:20 by aborboll          #+#    #+#             */
/*   Updated: 2021/07/25 11:32:15 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strdupdel(const char *s1)
{
	char	*str;
	int		i;

	if (s1 == NULL)
		return (0);
	i = ft_strlen(s1);
	str = (char *)malloc(sizeof(*s1) * (i + 1));
	if (!str)
		return (NULL);
	ft_memcpy(str, s1, i);
	str[i++] = '\0';
	ft_strdel(&s1);
	return (str);
}