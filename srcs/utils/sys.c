/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 18:06:58 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/13 18:10:46 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*getCurrentDir(char *path)
{
	char *token;
	char *directory;
	size_t length;

	token = ft_strrchr(path, '/');

	if (token == NULL)
		ft_error("Error getting dir", 1); /* You decide here */
	length = ft_strlen(token);
	directory = malloc(length);
	ft_memcpy(directory, token + 1, length);
	return directory;
}
