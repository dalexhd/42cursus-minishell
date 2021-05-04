/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 07:32:52 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/04 21:35:03 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*
** Include our libraries.
*/
# include "../libft/includes/libft.h"

/*
** Include necessary libraries.
*/
# include <stdarg.h>

/*
** Include internal values of the cub3d.
*/
# include "internal.h"

typedef struct s_shell
{
	t_bool	running;
	char	**envp;
}				t_shell;

t_shell	init_shell(char **envp);

/*
** Define builtins
*/
int		ft_echo(char **args, size_t size);
int		ft_pwd();
int		ft_env(char **envp);
int		ft_cd(char *dir, char **envp);
#endif
