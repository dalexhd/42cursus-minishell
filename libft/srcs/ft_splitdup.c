/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splitdup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 13:42:14 by aborboll          #+#    #+#             */
/*   Updated: 2021/07/22 17:07:44 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	**ft_split_dup(char **s)
{
	size_t	i;
	size_t	split_len;
	char	**words;

	split_len = ft_splitlen(s);
	words = ft_calloc(split_len + 1, sizeof(char *));
	i = 0;
	while (i < split_len)
	{
		words[i] = ft_strdup(s[i]);
		i++;
	}
	words[i] = NULL;
	return (words);
}
