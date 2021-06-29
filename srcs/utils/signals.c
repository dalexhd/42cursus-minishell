#include "../../includes/minishell.h"

static	void	handle_sigint(int sig)
{
	(void)sig;
	ft_fprintf(STDOUT_FILENO, "\n");
}

static	void	handle_sigquit(int sig)
{
	(void)sig;
}

static	void	handle_sigpipe(int sig)
{
	(void)sig;
	ft_fprintf(STDOUT_FILENO, "\n"C_GREEN "%s "C_GREEN "❯ " C_X,
		get_current_dir(ft_pwd()));
}

void	signal_handler(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	signal(SIGPIPE, handle_sigpipe);
}
