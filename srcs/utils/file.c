/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 10:25:58 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/17 17:28:58 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_bool	file_exists(char *filename)
{
	struct stat	buffer;

	return (stat(filename, &buffer) == 0);
}

char	*getCurrentDir(char *path)
{
	char	*token;
	char	*directory;
	size_t	length;

	token = ft_strrchr(path, '/');
	if (token == NULL)
		ft_error("Error getting dir", 1);
	length = ft_strlen(token);
	directory = malloc(length);
	ft_memcpy(directory, token + 1, length);
	return (directory);
}
