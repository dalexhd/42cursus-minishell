/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 18:21:08 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/06 20:47:02 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_cd(t_shell shell, char *dir)
{
	char	*pwd;

	pwd = ft_pwd();
	ft_export(shell, "OLDPWD", pwd);
	ft_strdel(&pwd);
	chdir(dir);
	pwd = ft_pwd();
	ft_export(shell, "PWD", pwd);
	ft_strdel(&pwd);
}
