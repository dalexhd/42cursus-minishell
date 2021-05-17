/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 18:06:58 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/17 17:20:01 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exec(t_shell *shell, t_parsed *parsed)
{
	char	*arg;

	arg = parsed->args[0];
	if (ft_isbuiltin(arg))
	{
		if (!ft_strcmp(arg, "cd"))
			ft_cd(shell, parsed->args[1]);
		else if (!ft_strcmp(arg, "export"))
			ft_export(shell, "hola", "mundo");
		else if (!ft_strcmp(arg, "env"))
			ft_env(shell);
		else if (!ft_strcmp(arg, "pwd"))
			ft_printf("%s\n", ft_pwd());
	}
	else if (execve(builtin_bin_path(shell, arg),
			parsed->args, shell->envp) == -1)
		ft_error("%s: command not found", 1, arg);
}
