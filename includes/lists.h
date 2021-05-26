/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/24 18:29:39 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/26 19:07:56 by aborboll         ###   ########.fr       */
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

typedef struct s_parsed
{
	t_list		*args;
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

#endif
