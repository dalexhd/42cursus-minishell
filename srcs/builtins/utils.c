/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/10 19:24:31 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/10 19:29:26 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_bool	ft_isbuiltin(char *builtin)
{
	return (!ft_strcmp(builtin, "ls")
		|| !ft_strcmp(builtin, "cd")
		|| !ft_strcmp(builtin, "env")
		|| !ft_strcmp(builtin, "exit")
		|| !ft_strcmp(builtin, "export")
		|| !ft_strcmp(builtin, "pwd")
		|| !ft_strcmp(builtin, "unset")
		|| !ft_strcmp(builtin, "unset"));
}
