/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 13:42:14 by aborboll          #+#    #+#             */
/*   Updated: 2021/04/25 15:33:28 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	**ft_split(char const *s, char c)
{
	t_uint	i;
	t_uint	j;
	char	**words;

	words = (char **)malloc(sizeof(char *) * (ft_countwords(s, c) + 1));
	if (s == NULL || !words)
		return (NULL);
	j = 0;
	while (*s != '\0')
	{
		i = 0;
		while (s[i] != c && s[i] != '\0')
			i++;
		if (i > 0)
		{
			words[j++] = ft_strncpy(ft_strnew(i), s, i);
			s = &s[i];
		}
		else
			s++;
	}
	words[j] = NULL;
	return (words);
}
