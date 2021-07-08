/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 13:42:23 by aborboll          #+#    #+#             */
/*   Updated: 2021/04/15 18:45:48 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*res;

	res = ft_strnew(ft_strlen(s1) + ft_strlen(s2));
	if (!res)
		return (0);
	res = ft_strcat(ft_strcpy(res, s1), s2);
	ft_strdel(&s1);
	ft_strdel(&s2);
	return (res);
}
