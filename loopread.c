#include <termcap.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include <term.h>
#include <errno.h>

//printf, malloc, free, write, open, read, close,
//fork, wait, waitpid, wait3, wait4, signal, kill,
//exit, getcwd, chdir, stat, lstat, fstat, execve,
//dup, dup2, pipe, opendir, readdir, closedir,
//strerror, errno, isatty, ttyname, ttyslot, ioctl,
//getenv, tcsetattr, tcgetattr, tgetent, tgetflag,
//tgetnum, tgetstr, tgoto, tputs

typedef struct s_config
{
	char			*term_name;
	char			text[2048];
	int				pos;
	struct termios	termios_raw;
	struct termios	termios_new;
}				t_config;

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	if (s)
	{
		i = 0;
		while (s[i] != '\0')
		{
			write(fd, &s[i], sizeof(s[i]));
			i++;
		}
	}
}

int	ft_putchar(int c)
{
	return (write(1, &c, 1));
}

void	die(char *msg)
{
	perror(msg);
	exit (1);
}

void	end_tc(t_config *e)
{
	write(STDOUT_FILENO, "end tc\n", 7);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &e->termios_raw);
	exit(1);
}

void	init_tc(t_config *e)
{
	e->term_name = getenv("TERM");
	tgetent(NULL, e->term_name);
	if (tcgetattr(STDIN_FILENO, &e->termios_raw) == -1)
		die("tcgetattr");
	e->termios_new = e->termios_raw;
	e->termios_new.c_iflag &= ~(BRKINT | ICRNL | IXON);
	e->termios_new.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	e->termios_new.c_cc[VMIN] = 1;
	e->termios_new.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &e->termios_new) == -1)
		die("tcsetattr");
	tputs(tgetstr("ks", NULL), 1, ft_putchar);
}

void	eraser(t_config *e)
{
	tputs(tgetstr("le", NULL), 1, ft_putchar);
	tputs(tgetstr("dc", NULL), 1, ft_putchar);
}

void	tear(t_config *e, char c)
{
//	write(STDOUT_FILENO, &c, 1);
	e->text[e->pos] = c;
//	e->text[e->pos] = 0;
	write(STDOUT_FILENO, &e->text[e->pos], 1);
//	ft_putstr_fd(e->text, 1);
//	write(STDOUT_FILENO, "\n", 1);
}

void	loureed(t_config *e)
{
	char	buf[5];

	e->pos = 0;
	while (read(STDIN_FILENO, &buf, 5) > 0)
	{
		if (buf[0] == 'D' - 64)
			end_tc(e);
		else if (buf[0] == 127)
			eraser(e);
//		else if (buf[0] == '\\' - 64)
//			end_tc(e);
		else if (buf[0] == 'C' - 64)
			tputs(tgetstr("cl", NULL), 1, ft_putchar);
		else if (!strcmp(buf, tgetstr("ku", NULL)))
			write(STDOUT_FILENO, "historial arriba\n", 17);
		else if (!strcmp(buf, tgetstr("kd", NULL)))
			write(STDOUT_FILENO, "historial abajo\n", 16);
		else if (buf[0] == 'M' - 64)
			write(STDOUT_FILENO, "\n", 1);
		else if (buf[0] > 31 && buf[0] < 127 && buf[1] == 0)
			tear(e, buf[0]);
		bzero(&buf, 5);
	}
}

int	main(int arc, char **arv, char **env)
{
	t_config	e;

	init_tc(&e);
//	write(STDOUT_FILENO, "hellshell$ ", 12);
	loureed(&e);
	end_tc(&e);
	return (0);
}
