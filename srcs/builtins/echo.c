/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 18:21:08 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/04 21:24:40 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_echo(char **args, size_t size)
{
	size_t  i;

	i = 0;
	while (i < size)
	{	
		if (args[i + 1])
			ft_printf("%s ", args[i]); 
		else
			ft_printf("%s", args[i]); 
		i++;
	}
	return (0);
}
