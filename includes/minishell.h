/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 07:32:52 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/18 18:30:17 by aborboll         ###   ########.fr       */
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
# include <sys/stat.h>
# include <signal.h>
/*
** Include internal values of the cub3d.
*/
# include "internal.h"
# include "lists.h"

/*
** Here we define or unique global variable. We need it,
** so the signal hander works...
*/
volatile t_bool	g_running;

typedef struct s_shell
{
	t_bool	should_wait;
	char	**envp;
	size_t	pipe_count;
	size_t	pid;
	t_bool	first;
	t_slist	*parsed;
	t_list	*history;
}				t_shell;

t_shell	*init_shell(char **envp);
void	exec_shell(t_shell *shell, char *cmd);

/*
** Define builtins
*/
int		ft_echo(char **args, size_t size);
char	*ft_pwd(void);
void	ft_env(t_shell *shell);
void	ft_cd(t_shell *shell, char *dir);
void	ft_export(t_shell *shell, char *env, char *value);
void	ft_unset(t_shell *shell, char *env);

/*
** Define builtins helpers
*/
char	*ft_getenv(t_shell *shell, char *env);

/*
** Define utils
*/
t_bool	file_exists(char *filename);
char	*getCurrentDir(char *path);

/*
** Parsing
*/
void	parse_line(t_shell *shell, char *cmd);
#endif
