/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 16:34:22 by aborboll          #+#    #+#             */
/*   Updated: 2021/04/15 18:47:14 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_printf.h"

int	apply_negative(t_info *info)
{
	char	*tmp;

	if (info->res[0] != '-')
		return (0);
	tmp = info->res;
	info->res = ft_strdup(++(info->res));
	if (!info->res)
		return (0);
	free(tmp);
	return (1);
}
