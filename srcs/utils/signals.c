/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 10:43:13 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/20 20:44:17 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static	void	handle_sigint(int sig)
{
	(void)sig;
	ft_fprintf(STDOUT_FILENO, "\n"C_GREEN "%s "C_GREEN "❯ " C_X,
		getCurrentDir(ft_pwd()));
}

static	void	handle_sigpipe(int sig)
{
	(void)sig;
	ft_fprintf(STDOUT_FILENO, "\n"C_GREEN "%s "C_GREEN "❯ " C_X,
		getCurrentDir(ft_pwd()));
}

void	signal_handler(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGPIPE, handle_sigpipe);
}
