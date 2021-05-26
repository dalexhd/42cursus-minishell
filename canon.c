#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <term.h>
#include <termios.h>

//printf, malloc, free, write, open, read, close,
//fork, wait, waitpid, wait3, wait4, signal, kill,
//exit, getcwd, chdir, stat, lstat, fstat, execve,
//dup, dup2, pipe, opendir, readdir, closedir,
//strerror, errno, isatty, ttyname, ttyslot, ioctl,
//getenv, tcsetattr, tcgetattr, tgetent, tgetflag,
//tgetnum, tgetstr, tgoto, tputs

int	main(int arc, char **arv, char ** envp)
{
	int	b;
	char	*s;

	tcflow(1, TCOOFF);
	b = read(1, &s, 10); 
	return (0);
}
