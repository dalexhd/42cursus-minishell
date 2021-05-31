/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 18:21:08 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/29 12:44:33 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_cd(t_shell *shell, char **args)
{
	char	*pwd;
	char	*dir;

	pwd = ft_pwd();
	if (!args[1])
		dir = ft_getenv(shell, "HOME");
	else
		dir = args[1];
	if (chdir(dir) == -1)
	{
		ft_strdel(&pwd);
		ft_error("cd: %s: No such file or directory\n", 0, dir);
		return ;
	}
	ft_export_internal(shell, "OLDPWD", pwd);
	ft_strdel(&pwd);
	pwd = ft_pwd();
	ft_export_internal(shell, "PWD", pwd);
	ft_strdel(&pwd);
}
