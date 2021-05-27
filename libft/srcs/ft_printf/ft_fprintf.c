/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/24 18:43:19 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/27 17:05:21 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	ft_fprintf(int fd, char *format, ...)
{
	va_list	args;
	int		result;
	t_info	*info;

	info = (t_info *)ft_strnew(sizeof(t_info));
	if (!info || !(format))
		return (EXIT_FAILURE);
	va_start(args, format);
	init_struct(info, &format, &args, fd);
	while (info->str[0][0])
	{
		if (info->str[0][0] != '%')
		{
			ft_putchar_fd(info->str[0][0], info->std);
			(info->str[0])++;
			info->count++;
		}
		else
			check_and_print(info);
	}
	result = info->count;
	free(info);
	va_end(args);
	return (result);
}
