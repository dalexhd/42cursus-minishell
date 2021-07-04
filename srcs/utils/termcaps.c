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
			C_GREEN "\n%s " C_GREEN "(%i)❯ " C_X, dir, shell->status);
		shell->term.new_line = false;
	}
	else
		ft_fprintf(STDOUT_FILENO,
			C_GREEN "%s " C_GREEN "(%i)❯ " C_X, dir, shell->status);
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
	//ft_hlstadd_front(&shell->term.history, ft_hlstnew(""));
	shell->term.term_name = getenv("TERM");
	tgetent(NULL, shell->term.term_name);
	if (tcgetattr(STDIN_FILENO, &shell->term.termios_raw) == -1)
		die("tcgetattr");
	enable_raw_mode(shell);
	tputs(tgetstr("ks", NULL), 1, ft_iputchar);
}

void	loureed(t_shell *shell)
{
	char	buf[4];
	int		ret;

	ft_bzero(&buf, 4);
	while (g_running)
	{
		while (1)
		{
			enable_raw_mode(shell);
			ret = read(STDIN_FILENO, &buf, 4);
			if (ret == -1)
				ft_error("Read error #332", true);
			if (buf[0] == 'D' - 64)
			{
				ctld(shell);
				break ;
			}
			else if (buf[0] == 127)
				eraser(shell);
			else if (buf[0] == '\\' - 64)
				ctlb(shell);
			else if (buf[0] == 'C' - 64)
				ctlc(shell);
			else if (buf[0] == 'L' - 64)
				ctll(shell);
			else if (tgetstr("ku", NULL) && !ft_strcmp(buf, tgetstr("ku", NULL)))
				history_up(shell);
			else if (tgetstr("kd", NULL) && !ft_strcmp(buf, tgetstr("kd", NULL)))
				history_down(shell);
			else if (buf[0] == 'M' - 64)
			{
				end_tc(shell);
				sandman(shell);
				break ;
			}
			else if (buf[0] > 31 && buf[0] < 127 && buf[1] == 0)
				tear(shell, buf[0]);
			ft_bzero(&buf, 4);
		}
		end_tc(shell);
	}
}
