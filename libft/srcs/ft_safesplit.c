/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safesplit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/24 10:05:54 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/24 16:55:23 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static	t_bool	ft_aretagclosed(t_bool *open)
{
	int		i;
	t_bool	closed;

	i = 0;
	closed = true;
	while (open[i] != '\0')
	{
		if (open[i] == true)
		{
			closed = false;
			break ;
		}
		i++;
	}
	return (closed);
}

static	int	ft_safecountwords(char const *str, char c, char *set)
{
	int		count;
	size_t	i;
	int		u;
	t_bool	*open;

	open = (t_bool *)malloc(sizeof(t_bool) * (ft_strlen(set) + 1));
	i = 0;
	while (i < ft_strlen(set))
	{
		open[i] = false;
		i++;
	}
	i = 0;
	count = 0;
	while (str[i])
	{
		u = 0;
		while (set[u])
		{
			if (str[i] == set[u])
				open[(int)(ft_strchr(set, set[u]) - set)]
					= !open[(int)(ft_strchr(set, set[u]) - set)];
			u++;
		}
		if (str[i] == c && ft_aretagclosed(open))
			count++;
		i++;
	}
	free(open);
	return (count);
}

static	t_uint	*ft_safesplitswords(char const *str, char c, char *set, int size)
{
	t_uint	i;
	int		u;
	int		d;
	t_uint	*count;
	t_bool	*open;

	open = (t_bool *)malloc(sizeof(t_bool) * (ft_strlen(set) + 1));
	count = (t_uint *)malloc(sizeof(t_uint) * (size + 1));
	i = 0;
	while (i < ft_strlen(set))
	{
		open[i] = false;
		i++;
	}
	i = 0;
	d = 0;
	while (str[i])
	{
		u = 0;
		while (set[u])
		{
			if (str[i] == set[u])
				open[(int)(ft_strchr(set, set[u]) - set)]
					= !open[(int)(ft_strchr(set, set[u]) - set)];
			u++;
		}
		if (str[i] == c && ft_aretagclosed(open))
		{
			count[d] = i;
			d++;
		}
		i++;
	}
	free(open);
	return (count);
}

char	**ft_safesplit(char const *s, char c, char *set)
{
	t_uint	i;
	t_uint	j;
	int		a;
	int		count;
	t_uint	*splits;
	char	**words;

	count = ft_safecountwords(s, c, set);
	words = (char **)malloc(sizeof(char *) * (count + 1));
	splits = ft_safesplitswords(s, c, set, count);
	if (s == NULL || !words)
		return (NULL);
	j = 0;
	i = 0;
	while (s[i] != '\0')
	{
		a = 0;
		while (splits[a])
		{
			if (i == splits[a])
			{
				words[j++] = ft_strtrim(ft_strncpy(ft_strnew(i), s, i), "\"'");
				s = &s[i];
				break ;
			}
			a++;
		}
		i++;
	}
	if (ft_strlen(s) > 1)
		words[j++] = ft_strtrim(ft_strncpy(ft_strnew(i), s, i), " \"'");
	words[j] = NULL;
	free(splits);
	return (words);
}
