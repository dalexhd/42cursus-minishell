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
