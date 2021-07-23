#include "../../includes/minishell.h"

static void	die(char *msg)
{
	ft_error("%s\n", 1, msg);
}

void	ft_printshell(t_shell *shell)
{
	char	*pwd;
	char	*dir;

	pwd = ft_pwd();
	dir = get_current_dir(pwd);
	if (shell->term.new_line)
	{
		ft_fprintf(STDOUT_FILENO,
			C_GREEN "\n%s " C_GREEN "(%i)> " C_X, dir, shell->status);
		shell->term.new_line = false;
	}
	else
		ft_fprintf(STDOUT_FILENO,
			C_GREEN "%s " C_GREEN "(%i)> " C_X, dir, shell->status);
	free(dir);
	free(pwd);
}

void	end_tc(t_shell *shell)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell->term.termios_raw);
}

void	enable_raw_mode(t_shell *shell)
{
	shell->term.termios_new = shell->term.termios_raw;
	shell->term.termios_new.c_iflag &= ~(BRKINT | ICRNL | IXON);
	shell->term.termios_new.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	shell->term.termios_new.c_cc[VMIN] = 1;
	shell->term.termios_new.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell->term.termios_new) == -1)
		die("tcsetattr");
}

void	init_tc(t_shell *shell)
{
	ft_bzero(&shell->term, sizeof(t_term));
	shell->term.cursor = 11;
	shell->term.pos = 0;
	old(shell);
	ft_hlstadd_front(&shell->term.history, ft_hlstnew(""));
	shell->term.term_name = getenv("TERM");
	tgetent(NULL, shell->term.term_name);
	if (tcgetattr(STDIN_FILENO, &shell->term.termios_raw) == -1)
		die("tcgetattr");
	enable_raw_mode(shell);
	tputs(tgetstr("ks", NULL), 1, ft_iputchar);
}
