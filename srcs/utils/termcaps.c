/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 18:48:04 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/20 20:47:38 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static	void	die(char *msg)
{
	ft_error("%s\n", 1, msg);
}

static	void	tear(t_shell *shell, char c)
{
	write(STDOUT_FILENO, &c, 1);
	shell->term.line[shell->term.pos++] = c;
	shell->term.line[shell->term.pos] = 0;
	shell->term.cursor++;
	ft_strcpy(shell->term.aux, shell->term.line);
}

static	void	ctld(t_shell *shell)
{
	if (!*shell->term.line)
		ft_success("exit\n", 1);
}

void	ft_printshell(t_shell *shell)
{
	if (shell->first)
		ft_fprintf(STDOUT_FILENO,
			C_CYAN "Wellcome to our minishell 😋" C_GREEN "❯ " C_X);
	else if (shell->term.new_line)
	{
		ft_fprintf(STDOUT_FILENO,
			C_GREEN "\n%s "C_GREEN "❯ " C_X, getCurrentDir(ft_pwd()));
		shell->term.new_line = false;
	}
	else
		ft_fprintf(STDOUT_FILENO,
			C_GREEN "%s "C_GREEN "❯ " C_X, getCurrentDir(ft_pwd()));
}

static	void	ctlc(t_shell *shell)
{
	ft_bzero(&shell->term.line, BUFF_SIZE);
	shell->term.pos = 0;
	shell->term.new_line = true;
	ft_printshell(shell);
	shell->term.cursor = 11;
}

static	void	ctlb(t_shell *shell)
{
	ft_bzero(&shell->term.line, BUFF_SIZE);
	shell->term.pos = 0;
	tputs(tgetstr("cr", NULL), 1, ft_iputchar);
	tputs(tgetstr("dl", NULL), 1, ft_iputchar);
	ft_printshell(shell);
	shell->term.cursor = 11;
}

void	end_tc(t_shell *shell)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell->term.termios_raw);
}

static	void	eraser(t_shell *shell)
{
	if (shell->term.cursor > 11)
	{
		tputs(tgetstr("le", NULL), 1, ft_iputchar);
		tputs(tgetstr("dc", NULL), 1, ft_iputchar);
		shell->term.cursor--;
		shell->term.line[--shell->term.pos] = 0;
		ft_strcpy(shell->term.aux, shell->term.line);
	}
}

static	void	history_up(t_shell *shell) // History up
{
	if (!shell->term.history)
		return ;
	tputs(tgetstr("cr", NULL), 1, ft_iputchar);
	tputs(tgetstr("dl", NULL), 1, ft_iputchar);
	ft_printshell(shell);
	ft_strcpy(shell->term.line, shell->term.history->content);
	ft_putstr_fd(shell->term.history->content, 1);
	shell->term.pos = ft_strlen(shell->term.history->content);
	shell->term.cursor = 11 + shell->term.pos;
	if (shell->term.history->next)
		shell->term.history = shell->term.history->next;
}

//cargar historial en line /////
//guardar line en aux/////
//mostrar aux al volver al inicio de historial/////
//comprobar que la instrucion no se repite
//apuntar a principio al pulsar enter/////

static	void	history_down(t_shell *shell) // History down
{
	if (!shell->term.history)
		return ;
	if (shell->term.history->prev)
	{
		tputs(tgetstr("cr", NULL), 1, ft_iputchar);
		tputs(tgetstr("dl", NULL), 1, ft_iputchar);
		ft_printshell(shell);
		ft_strcpy(shell->term.line, shell->term.history->prev->content);
		ft_fprintf(STDOUT_FILENO, shell->term.history->prev->content);
		shell->term.pos = ft_strlen(shell->term.history->prev->content);
		shell->term.cursor = 11 + shell->term.pos;
		shell->term.history = shell->term.history->prev;
	}
	else
	{
		tputs(tgetstr("cr", NULL), 1, ft_iputchar);
		tputs(tgetstr("dl", NULL), 1, ft_iputchar);
		ft_printshell(shell);
		ft_strcpy(shell->term.line, shell->term.aux);
		ft_fprintf(STDOUT_FILENO, shell->term.line);
		shell->term.pos = ft_strlen(shell->term.line);
		shell->term.cursor = 11 + shell->term.pos;
	}
}

static	void	sandman(t_shell *shell)
{
	if (*shell->term.line)
	{
		shell->term.line[shell->term.pos] = 0;
		shell->term.history = ft_lstfirst(shell->term.history);
		if  (!shell->term.history)
			ft_lstadd_front(&shell->term.history, ft_lstnew(ft_strdup(shell->term.line)));
		else
			if (ft_strlen(shell->term.line) > 0 && ft_strcmp(shell->term.history->content, shell->term.line) != 0)
				ft_lstadd_front(&shell->term.history, ft_lstnew(ft_strdup(shell->term.line)));
	}
	ft_putchar_fd('\n', STDOUT_FILENO);
	exec_shell(shell, shell->term.line);
	ft_bzero(&shell->term.line, BUFF_SIZE);
	ft_bzero(&shell->term.aux, BUFF_SIZE);
	shell->term.pos = 0;
	if (g_running)
	{
		ft_printshell(shell);
	}
	shell->term.cursor = 11;

}

void	loureed(t_shell *shell)
{
	char	buf[4];

	shell->term.pos = 0;
	ft_bzero(&buf, 4);
	while (g_running && read(STDIN_FILENO, &buf, 4) > 0)
	{
		if (buf[0] == 'D' - 64)
			ctld(shell);
		else if (buf[0] == 127)
			eraser(shell);
		else if (buf[0] == '\\' - 64)
			ctlb(shell);
		else if (buf[0] == 'C' - 64)
			ctlc(shell);
		else if (tgetstr("ku", NULL) && !ft_strcmp(buf, tgetstr("ku", NULL)))
			history_up(shell);
		else if (tgetstr("kd", NULL) && !ft_strcmp(buf, tgetstr("kd", NULL)))
			history_down(shell);
		else if (buf[0] == 'M' - 64)
			sandman(shell);
		else if (buf[0] > 31 && buf[0] < 127 && buf[1] == 0)
			tear(shell, buf[0]);
		ft_bzero(&buf, 4);
	}
}

void	init_tc(t_shell *shell)
{
	ft_bzero(&shell->term, sizeof(t_term));
	shell->term.cursor = 12;
	shell->term.term_name = getenv("TERM");
	tgetent(NULL, shell->term.term_name);
	if (tcgetattr(STDIN_FILENO, &shell->term.termios_raw) == -1)
		die("tcgetattr");
	shell->term.termios_new = shell->term.termios_raw;
	shell->term.termios_new.c_iflag &= ~(BRKINT | ICRNL | IXON);
	shell->term.termios_new.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	shell->term.termios_new.c_cc[VMIN] = 1;
	shell->term.termios_new.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell->term.termios_new) == -1)
		die("tcsetattr");
	tputs(tgetstr("ks", NULL), 1, ft_iputchar);
}
