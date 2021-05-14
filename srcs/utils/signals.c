/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/14 10:43:13 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/14 17:36:32 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	void	handle_sigint()
{
	ft_fprintf(STDOUT_FILENO, "\n"C_GREEN "%s "C_GREEN "❯ " C_X, getCurrentDir(ft_pwd()));
}

void	signal_handler(void)
{
	signal(SIGINT, handle_sigint);
}
