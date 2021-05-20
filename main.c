#include "includes/minishell.h"

//printf, malloc, free, write, open, read, close,
//fork, wait, waitpid, wait3, wait4, signal, kill,
//exit, getcwd, chdir, stat, lstat, fstat, execve,
//dup, dup2, pipe, opendir, readdir, closedir,
//strerror, errno, isatty, ttyname, ttyslot, ioctl,
//getenv, tcsetattr, tcgetattr, tgetent, tgetflag,
//tgetnum, tgetstr, tgoto, tputs

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	(void)argc;
	(void)**argv;
	shell = init_shell(envp);
	ft_printshell(shell);
	loureed(shell);
	end_tc(shell);
	return (0);
}
