/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/24 18:29:39 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/25 02:43:40 by aborboll         ###   ########.fr       */
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
}				t_redirect;

typedef struct s_flags
{
	t_bool		has_stdin;
	t_bool		has_stdout;
	t_bool		has_pipe;
	t_redirect	redirect;
}				t_flags;

typedef struct s_parsed
{
	char	**args;
	char	*line;
	pid_t	pid;
	t_flags	flags;
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

t_slist			*ft_slstnew(t_parsed *content);
void			ft_slstadd_front(t_slist **alst, t_slist *new);
int				ft_slstsize(t_slist *lst);
t_slist			*ft_slstlast(t_slist *lst);
void			ft_slstadd_back(t_slist **alst, t_slist *new);
void			ft_slstdelone(t_slist *lst, void (*del)(t_parsed *));
void    		ft_slstclear(t_slist **lst, void (*del)(t_parsed *));
void			ft_slstiter(t_slist *lst, void (*f)(t_parsed *));

#endif
