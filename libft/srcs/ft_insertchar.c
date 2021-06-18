/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_insertchar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 17:25:25 by aborboll          #+#    #+#             */
/*   Updated: 2021/06/18 17:27:01 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_insertchar(char *s, char c, size_t pos)
{
	size_t	i;
	char	tmp;

	i = 0;
	while (i < pos && s[i])
		i++;
	if (i == pos)
	{
		while (c)
		{
			tmp = s[i];
			s[i] = c;
			c = tmp;
			i++;
		}
		s[i] = c;
	}
	return (s);
}
