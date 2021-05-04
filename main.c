#include "includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	int		i;
	(void)argc;
	(void)**argv;
	
	shell = init_shell(envp);
	i = 0;
	while (shell.running)
	{
		i++;
		if (i == 10)
			shell.running = false;
	}
	ft_success("Nice 🧡", 1);
	return (0);
}