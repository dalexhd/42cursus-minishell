/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 07:32:52 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/26 21:27:34 by aborboll         ###   ########.fr       */
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
# include <term.h>
# include <termios.h>
# include <termcap.h>
# include <string.h>

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

typedef struct s_term
{
	char			*term_name;
	char			line[2048];
	t_hist			*history;
	int				pos;
	int				cursor;
	struct termios	termios_raw;
	struct termios	termios_new;
	t_bool			new_line;
}				t_term;

typedef struct s_shell
{
	t_bool	should_wait;
	char	**envp;
	size_t	pipe_count;
	size_t	pid;
	t_bool	first;
	t_slist	*parsed;
	t_term	term;
}				t_shell;

t_shell	*init_shell(char **envp);
void	exec_shell(t_shell *shell, char *cmd);
void	signal_handler(void);
char	*parse_line(t_shell *shell, char *cmd);

/*
** Define builtins
*/
int		ft_echo(char **args, size_t size);
char	*ft_pwd();
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
t_bool	ft_isbuiltin(char *builtin);
t_bool	file_exists(char *filename);
char	*builtin_bin_path(t_shell *shell, char *builtin);
char	*getCurrentDir(char *path);
void	exec(t_shell *shell, t_parsed *parsed);
void	run(t_shell *shell);

/*
** Define termcaps utils
*/
void	init_tc(t_shell *shell);
void	end_tc(t_shell *shell);
void	loureed(t_shell *shell);
void	ft_printshell(t_shell *shell);
#endif
