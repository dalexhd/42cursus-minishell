#include "../../includes/minishell.h"

void	ctld(t_shell *shell)
{
	if (!*shell->term.line)
	{
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell->term.termios_raw);
		ft_printf("exit\n");
		g_running = false;
	}
}

void	ctlc(t_shell *shell)
{
	ft_bzero(&shell->term.line, BUFF_SIZE);
	shell->term.history = ft_hlstfirst(shell->term.history);
	shell->term.pos = 0;
	shell->term.new_line = true;
	shell->status = 1;
	ft_printshell(shell);
	shell->term.cursor = 11;
}

void	ctll(t_shell *shell)
{
	tputs(tgetstr("cl", NULL), 1, ft_iputchar);
	ft_printshell(shell);
	ft_printf("%s", shell->term.line);
}

void	ctlb(t_shell *shell)
{
	ft_bzero(&shell->term.line, BUFF_SIZE);
	shell->term.pos = 0;
	tputs(tgetstr("cr", NULL), 1, ft_iputchar);
	tputs(tgetstr("dl", NULL), 1, ft_iputchar);
	ft_printshell(shell);
	shell->term.cursor = 11;
}
