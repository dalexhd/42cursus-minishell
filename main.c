#include "includes/minishell.h"

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
