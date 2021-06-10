#include "../../includes/minishell.h"

static	void	die(char *msg)
{
	ft_error("%s\n", 1, msg);
}

static	void	ctld(t_shell *shell)
{
	if (!*shell->term.line)
	{
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell->term.termios_raw);
		g_running = false;
		ft_success("exit\n", 0);
	}
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

static void	new(t_shell *shell)
{
	int		input;
	char	*tmp_dir;

	tmp_dir = ft_getenv(shell, "HOME");
	if (tmp_dir)
	{
		input = open(ft_strjoin(tmp_dir, "/.minishell_history"), O_CREAT | O_WRONLY | O_APPEND, 0600);
		write(input, ft_strjoin(shell->term.line, "\n"), ft_strlen(shell->term.line) + 1);
		close(input);
	}
}


static void	old(t_shell *shell)
{
	int		test;
	int		fd;
	char	*line;
	char	*tmp_dir;

	tmp_dir = ft_getenv(shell, "HOME");
	if (tmp_dir)
	{
		fd = open(ft_strjoin(tmp_dir, "/.minishell_history"), O_CREAT | O_RDONLY, 0600);
		test = get_next_line(fd, &line);
		while(test)
		{
			ft_hlstadd_front(&shell->term.history, ft_hlstnew(ft_strdup(line)));
			shell->term.history->original = ft_strdup(line);
			shell->term.history->copy = ft_strdup(line);
			test = get_next_line(fd, &line);
		}
		close (fd);
	}
}

void	end_tc(t_shell *shell)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell->term.termios_raw);
}

void	init_tc(t_shell *shell)
{
	ft_bzero(&shell->term, sizeof(t_term));
	shell->term.cursor = 11;
	old(shell);
	ft_hlstadd_front(&shell->term.history, ft_hlstnew(ft_strdup("")));
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

static	void	eraser(t_shell *shell)
{
	if (shell->term.cursor > 11)
	{
		tputs(tgetstr("le", NULL), 1, ft_iputchar);
		tputs(tgetstr("dc", NULL), 1, ft_iputchar);
		shell->term.cursor--;
		shell->term.line[--shell->term.pos] = 0;
	}
}

static	void	history_up(t_shell *shell)
{
	if (shell->term.history->next)
	{
		tputs(tgetstr("cr", NULL), 1, ft_iputchar);
		tputs(tgetstr("dl", NULL), 1, ft_iputchar);
		ft_printshell(shell);
		free(shell->term.history->copy);
		shell->term.history->copy = ft_strdup(shell->term.line);
		shell->term.history = shell->term.history->next;
		ft_strcpy(shell->term.line, shell->term.history->copy);
		ft_putstr_fd(shell->term.history->copy, 1);
		shell->term.pos = ft_strlen(shell->term.history->copy);
		shell->term.cursor = 11 + shell->term.pos;
	}
}

static	void	history_down(t_shell *shell)
{
	if (!shell->term.history)
		return ;
	if (shell->term.history->prev)
	{
		tputs(tgetstr("cr", NULL), 1, ft_iputchar);
		tputs(tgetstr("dl", NULL), 1, ft_iputchar);
		ft_printshell(shell);
		free(shell->term.history->copy);
		shell->term.history->copy = ft_strdup(shell->term.line);
		shell->term.history = shell->term.history->prev;
		ft_strcpy(shell->term.line, shell->term.history->copy);
		ft_putstr_fd(shell->term.line, 1);
		shell->term.pos = ft_strlen(shell->term.line);
		shell->term.cursor = 11 + shell->term.pos;
	}
}

static	void	newliner(t_shell *shell)
{
	if (*shell->term.line)
	{
		shell->term.history->copy = ft_strdup(shell->term.history->original);
		shell->term.history = ft_hlstfirst(shell->term.history);
		free(shell->term.history->original);
		free(shell->term.history->copy);
		shell->term.history->original = ft_strdup(shell->term.line);
		shell->term.history->copy = ft_strdup(shell->term.line);
		new(shell);
		ft_hlstadd_front(&shell->term.history, ft_hlstnew(ft_strdup("")));
	}
}

static	void	sandman(t_shell *shell)
{
	t_list		*commands;

	newliner(shell);
	free(shell->term.history->copy);
	shell->term.history->copy = ft_strdup(shell->term.history->original);
	ft_putchar_fd('\n', STDOUT_FILENO);
	commands = ft_safesplitlist(shell->term.line, ';', "\"'");
	while (commands)
	{
		exec_shell(shell, commands->content);
		commands = commands->next;
	}

	tputs(tgetstr("ks", NULL), 1, ft_iputchar);
	ft_bzero(&shell->term.line, BUFF_SIZE);
	shell->term.history = ft_hlstfirst(shell->term.history);
	shell->term.pos = 0;
	if (g_running)
		ft_printshell(shell);
	shell->term.cursor = 11;
}

static	void	tear(t_shell *shell, char c)
{
	write(STDOUT_FILENO, &c, 1);
	shell->term.line[shell->term.pos++] = c;
	shell->term.line[shell->term.pos] = 0;
	shell->term.cursor++;
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
