/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/06 23:34:09 by aborboll          #+#    #+#             */
/*   Updated: 2021/04/17 16:10:46 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

static void	do_char_1(t_info *info)
{
	char	sarg;

	sarg = (char)va_arg(info->args[0], int);
	info->res = ft_strnew(1);
	if (sarg)
		info->res[0] = sarg;
	else
	{
		info->flags[F_Trash] = 1;
		info->count += 1;
	}
}

void	do_char(t_info *info)
{
	int		len;

	info->res = ft_strnew(1);
	do_char_1(info);
	len = ft_strlen(info->res);
	if (info->width > len)
	{
		while (info->flags[F_Neg] && len++ < info->width)
		{
			if (len + 1 == info->width && info->flags[F_Trash])
				info->res = ft_strjoin_free(info->res, ft_strdup(""));
			else
				info->res = ft_strjoin_free(info->res, ft_strdup(" "));
		}
		while (info->flags[F_Zero] && len++ < info->width)
			info->res = ft_strjoin_free(ft_strdup("0"), info->res);
		while (len++ < info->width)
		{
			if (len + 1 == info->width && info->flags[F_Trash])
				info->res = ft_strjoin_free(ft_strdup(""), info->res);
			else
				info->res = ft_strjoin_free(ft_strdup(" "), info->res);
		}
	}
}

void	do_percentage(t_info *info)
{
	int		len;

	info->res = ft_strdup("%");
	len = ft_strlen(info->res);
	if (info->width > len)
	{
		while (info->flags[F_Neg] && len < info->width)
		{
			info->res = ft_strjoin_free(info->res, ft_strdup(" "));
			len++;
		}
		while (info->flags[F_Zero] && len < info->width)
		{
			info->res = ft_strjoin_free(ft_strdup("0"), info->res);
			len++;
		}
		while (len < info->width)
		{
			info->res = ft_strjoin_free(ft_strdup(" "), info->res);
			len++;
		}
	}
}
