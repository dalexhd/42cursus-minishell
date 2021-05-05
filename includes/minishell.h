/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 07:32:52 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/05 21:41:34 by aborboll         ###   ########.fr       */
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
# include <errno.h>

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
char	*ft_pwd();
void	ft_env(t_shell shell);
void	ft_cd(t_shell shell, char *dir);
void	ft_export(t_shell shell, char *env, char *value);
void	ft_unset(t_shell shell, char *env);

/*
** Define builtins helpers
*/
char	*ft_getenv(t_shell shell, char *env);
#endif
