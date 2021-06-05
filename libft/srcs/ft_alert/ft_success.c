/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_success.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/21 15:15:10 by aborboll          #+#    #+#             */
/*   Updated: 2021/06/05 16:57:14 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	ft_success(char *s, t_bool force, ...)
{
	va_list	args;

	va_start(args, force);
	ft_vfprintf(STDOUT_FILENO, s, &args);
	va_end(args);
	if (force)
		exit(EXIT_SUCCESS);
}
