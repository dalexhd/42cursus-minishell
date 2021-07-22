/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 13:42:14 by aborboll          #+#    #+#             */
/*   Updated: 2021/07/22 17:34:48 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	*ft_realloc(void *ptr, size_t size)
{
	char	*new;

	if (!ptr)
		return (malloc(size));
	new = malloc(size);
	ft_memcpy(new, ptr, size);
	free(ptr);
	return (new);
}
