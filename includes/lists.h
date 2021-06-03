/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/24 18:29:39 by aborboll          #+#    #+#             */
/*   Updated: 2021/06/03 22:25:18 by aborboll         ###   ########.fr       */
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
}				t_rstatus;
typedef struct s_redirect
{
	t_rstatus	out;
	t_rstatus	in;
	t_rstatus	aout;
}				t_redirect;

typedef struct s_flags
{
	t_bool		has_stdin;
	t_bool		has_stdout;
	t_bool		has_pipe;
	t_redirect	redirect;
}				t_flags;

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
}				t_args;

typedef struct s_alist
{
	t_args			*content;
	struct s_alist	*next;
	struct s_alist	*prev;
}				t_alist;

typedef struct s_parsed
{
	t_alist		*args;
	char		*line;
	pid_t		pid;
	t_flags		flags;
	t_rlist		*redirects;
}				t_parsed;

typedef struct s_pipes
{
	size_t	count;
	t_list	*pos;
}				t_pipes;

typedef struct s_hist
{
	void			*original;
	void			*copy;
	struct s_hist	*next;
	struct s_hist	*prev;
}				t_hist;

typedef struct s_slist
{
	t_parsed		*content;
	struct s_slist	*next;
	struct s_slist	*prev;
}				t_slist;

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
void			ft_rlstclear(t_rlist **lst, void (*del)(t_redirect *));
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
t_alist			*ft_alstfirst(t_alist *lst);

#endif
