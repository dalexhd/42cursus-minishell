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
# include <sys/wait.h>
# include <signal.h>
# include <term.h>
# include <termios.h>
# include <termcap.h>
# include <string.h>
# include <dirent.h>

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
	t_pids	*pids;
	t_bool	first;
	t_bool	is_cmd;
	t_slist	*parsed;
	t_term	term;
	t_bool	force_stop;
	t_envp	*envp_2;
	int		status;
	char	*home_dir;
	char	*tmp_dir;
	t_alist	*mierdecilla;
}				t_shell;

t_shell		*init_shell(char **envp);
void		exec_shell(t_shell *shell, char *cmd);
void		signal_handler(void);
char		*parse_line(t_shell *shell, t_args *arg, char *cmd);
void		lsh_split_line(t_shell *shell, char *line);
void		clear_cmd(t_shell *shell);
/*
** Define builtins
*/
int			ft_echo(t_shell *shell, char **args);
char		*ft_pwd(void);
void		ft_env(t_shell *shell, char **args);
void		ft_cd(t_shell *shell, char **args);
void		ft_cd_internal(t_shell *shell, char *dir, t_bool set_home);
void		ft_export_internal(t_shell *shell, char *env, char *value);
void		ft_export(t_shell *shell, char **args);
void		ft_unset(t_shell *shell, char **args);
void		ft_exit(t_shell *shell, char **args);

/*
** Define builtins helpers
*/
t_envp		*ft_setup_env(t_shell *shell, char **envp);
void		*ft_getenv(t_shell *shell, char *key, t_bool ret_stuct);
t_env		*ft_parse_env(char *key, char *val);

/*
** Define utils
*/
t_bool		ft_isbuiltin(char *builtin);
int			ft_exec_builtin(t_shell *shell, t_slist *parsed);
void		ft_exec_bin(t_shell *shell, t_alist *args);
t_bool		file_exists(char *filename);
t_bool		is_directory(char *path);
t_bool		has_access(char *path);
char		*builtin_bin_path(t_shell *shell, char *bu);
char		*get_current_dir(char *path);
void		exec(t_shell *shell, t_parsed *parsed);
void		run(t_shell *shell);
char		*clean_str(t_shell *shell, t_args *arg, char *c);
t_aslist	*second_parse(t_shell *shell, t_parsed *parsed, t_aslist *tmp);
void		parse_args(t_shell *shell, t_parsed *parsed, char **cmd);
t_args		*parse_arg(t_shell *shell, t_alist *args, t_aslist *value);
char		**ft_safesplit(t_shell *shell, t_alist *list);
void		debug_log(char *string);
t_bool		has_redirect(t_rlist *redirect);
t_bool		arg_has_red(t_alist *arg);
t_bool		arg_is_readable(t_args *content);
void		filter_readable_args(t_slist *parsed);
void		ft_envlstfilter_exec(t_shell *shell);
int			get_quote_type(char *cmd);
t_aslist	*ft_safesplitlist(char *s, char c, char *st);
t_aslist	*ft_safesplitlist_new(char *s, char c, char *set);
void		sh_error(t_shell *shell, char *s, int force, ...);
void		ft_print_declare(t_shell *shell);
t_bool		valid_export(t_shell *shell, char *s, char **val);
void		shell_lvl(t_shell *shell);

/*
** Parsing
*/
void		parse_dollar(t_shell *shell, char **cmd, int *i, char **line);
void		parse_tilde(t_shell *shell, char *cmd, int *i, char **line);
char		*remove_cmd_quotes(char *cmd);
void		parse_commands(t_shell *shell, char *line);

/*
** Memory
*/
void		del_alst(t_args *args);
void		del_slst(t_parsed *parsed);
void		del_envlst(t_env *env);

/*
** Vallidation
*/
char		*fix_cmd(char *cmdi);
t_bool		valid_quotes(t_shell *shell, char **cmd, int *i);
t_bool		valid_redirects(t_shell *shell, char *cmd, int *i);
t_bool		validate_str(t_shell *shell, char **cmd);
t_bool		valid_commas(t_shell *shell, char *cmd);
t_bool		valid_pipes(t_shell *shell, char *cmd);

/*
** Define termcaps utils
*/
void		init_tc(t_shell *shell);
void		end_tc(t_shell *shell);
void		enable_raw_mode(t_shell *shell);
void		loureed(t_shell *shell);
void		ft_printshell(t_shell *shell);
void		ctld(t_shell *shell);
void		ctlb(t_shell *shell);
void		ctlc(t_shell *shell);
void		ctll(t_shell *shell);
void		tear(t_shell *shell, char c);
void		sandman(t_shell *shell);
void		newliner(t_shell *shell);
void		eraser(t_shell *shell);
void		new(t_shell *shell);
void		old(t_shell *shell);
void		history_up(t_shell *shell);
void		history_down(t_shell *shell);

#endif
