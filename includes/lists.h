/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/24 18:29:39 by aborboll          #+#    #+#             */
/*   Updated: 2021/08/06 10:00:37 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTS_H
# define LISTS_H

/*
** Incluimos las librerias esenciales.
*/

# include "minishell.h"

typedef struct s_srstatus
{
	t_bool	status;
	char	*file;
	int		fd;
}				t_rstatus;
typedef struct s_redirect
{
	t_rstatus	out;
	t_rstatus	in;
	t_rstatus	aout;
}				t_redirect;

typedef struct s_rlist
{
	t_redirect		*content;
	struct s_rlist	*next;
	struct s_rlist	*prev;
}				t_rlist;

typedef struct s_args
{
	int			type;
	char		*cmd;
	char		*file;
	t_bool		is_builtin;
	char		*bin_path;
	t_bool		is_literal;
	int			quot_type;
	t_bool		spaced;
	t_redirect	*redirect;
	t_bool		readable;
}				t_args;

typedef struct s_asrg
{
	char		*arg;
	t_bool		concat;
	char		next;
}				t_asrg;

typedef struct s_alist
{
	t_args			*content;
	struct s_alist	*next;
	struct s_alist	*prev;
}				t_alist;

typedef struct s_pids
{
	pid_t			*content;
	struct s_pids	*next;
	struct s_pids	*prev;
}				t_pids;

typedef struct s_parsed
{
	t_alist		*args;
	char		*line;
	pid_t		pid;
	t_bool		valid;
	t_rlist		*redirects;
}				t_parsed;

typedef struct s_env
{
	char		*key;
	t_bool		has_val;
	char		*val;
}				t_env;

typedef struct s_pipes
{
	size_t	count;
	t_list	*pos;
}				t_pipes;

typedef struct s_hist
{
	char			*original;
	char			*copy;
	struct s_hist	*next;
	struct s_hist	*prev;
}				t_hist;

typedef struct s_slist
{
	t_parsed		*content;
	struct s_slist	*next;
	struct s_slist	*prev;
}				t_slist;

typedef struct s_envp
{
	t_env			*content;
	struct s_envp	*next;
	struct s_envp	*prev;
}				t_envp;

typedef struct s_aslist
{
	t_asrg			*content;
	struct s_aslist	*next;
	struct s_aslist	*prev;
}				t_aslist;

/*
** Shell lists
*/
t_slist			*ft_slstnew(t_parsed *content);
void			ft_slstadd_front(t_slist **alst, t_slist *new);
int				ft_slstsize(t_slist *lst);
t_slist			*ft_slstlast(t_slist *lst);
void			ft_slstadd_back(t_slist **alst, t_slist *new);
void			ft_slstdelone(t_slist *lst, void (*del)(t_parsed *));
void			ft_slstclear(t_slist **lst, void (*del)(t_parsed *));
void			ft_slstiter(t_slist *lst, void (*f)(t_parsed *));

/*
** Redirects lists
*/
t_rlist			*ft_rlstnew(t_redirect *content);
void			ft_rlstadd_front(t_rlist **alst, t_rlist *new);
int				ft_rlstsize(t_rlist *lst);
t_rlist			*ft_rlstlast(t_rlist *lst);
void			ft_rlstadd_back(t_rlist **alst, t_rlist *new);
void			ft_rlstdelone(t_rlist *lst, void (*del)(t_redirect *));
void			ft_rlstclear(t_rlist **lst, void (*del)(void *));
void			ft_rlstiter(t_rlist *lst, void (*f)(t_redirect *));

/*
** History lists
*/
void			ft_hlstadd_back(t_hist **alst, t_hist *new);
void			ft_hlstadd_front(t_hist **alst, t_hist *new);
void			ft_hlstclear(t_hist **lst, void (*del)(void*));
void			ft_hlstdelone(t_hist *lst, void (*del)(void*));
t_hist			*ft_hlstfirst(t_hist *hst);
t_hist			*ft_hlstlast(t_hist *lst);
t_hist			*ft_hlstnew(void *content);
int				ft_hlstsize(t_hist *lst);

/*
** Pids lists
*/
void			ft_plstadd_back(t_pids **alst, t_pids *new);
void			ft_plstadd_front(t_pids **alst, t_pids *new);
void			ft_plstclear(t_pids **lst, void (*del)(void*));
void			ft_plstdelone(t_pids *lst, void (*del)(void*));
t_pids			*ft_plstfirst(t_pids *hst);
t_pids			*ft_plstlast(t_pids *lst);
t_pids			*ft_plstnew(pid_t *content);
int				ft_plstsize(t_pids *lst);

/*
** Arguments lists
*/
t_alist			*ft_alstnew(t_args *content);
void			ft_alstadd_front(t_alist **alst, t_alist *new);
int				ft_alstsize(t_alist *lst);
t_alist			*ft_alstlast(t_alist *lst);
void			ft_alstadd_back(t_alist **alst, t_alist *new);
void			ft_alstdelone(t_alist *lst, void (*del)(t_args *));
void			ft_alstclear(t_alist **lst, void (*del)(t_args *));
void			ft_alstiter(t_alist *lst, void (*f)(t_args *));
t_alist			*ft_alstmap(t_alist *lst, void *(*f)(t_args *),
					void (*del)(t_args *));
t_alist			*ft_alstfirst(t_alist *lst);
t_alist			*ft_alstfilter(t_alist *lst, t_bool (*f)(t_args *),
					void (*del)(t_args *));

/*
** Env lists
*/
t_envp			*ft_envlstnew(t_env *content);
void			ft_envlstadd_front(t_envp **envlst, t_envp *new);
int				ft_envlstsize(t_envp *lst);
t_envp			*ft_envlstlast(t_envp *lst);
void			ft_envlstadd_back(t_envp **envlst, t_envp *new);
void			ft_envlstdelone(t_envp *lst, void (*del)(t_env *));
void			ft_envlstclear(t_envp **lst, void (*del)(t_env *));
void			ft_envlstiter(t_envp *lst, void (*f)(t_env *));
t_envp			*ft_envlstmap(t_envp *lst, void *(*f)(t_env *),
					void (*del)(t_env *));
t_envp			*ft_envlstfirst(t_envp *lst);
t_envp			*ft_envlstfilter(t_envp *lst, t_bool (*f)(t_env *));
t_envp			*ft_envlstfilterarg(t_envp *lst,
					t_bool (*f)(t_env *, char *arg),
					void (*del)(t_env *), char *arg);
/*
** Single Arguments lists
*/
t_aslist		*ft_aslstnew(t_asrg *content);
void			ft_aslstadd_front(t_aslist **alst, t_aslist *new);
int				ft_aslstsize(t_aslist *lst);
t_aslist		*ft_aslstlast(t_aslist *lst);
void			ft_aslstadd_back(t_aslist **alst, t_aslist *new);
void			ft_aslstdelone(t_aslist *lst, void (*del)(t_asrg *));
void			ft_aslstclear(t_aslist **lst, void (*del)(void *));
void			ft_aslstiter(t_aslist *lst, void (*f)(t_asrg *));
t_aslist		*ft_aslstfirst(t_aslist *lst);

#endif
