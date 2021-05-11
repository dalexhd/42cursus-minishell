#include "includes/minishell.h"

void	ft_clear_memory(t_shell *shell)
{
	int		i;

	i = 0;
	while (shell->envp[i] != 0)
	{
		//ft_printf("%s\n", shell.envp[i]);
		//free(shell.envp[i]);
		i++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	int		i;

	(void)argc;
	(void)**argv;
	shell = init_shell(argv[1], envp);
	i = 0;
	while (shell->running)
	{
		i++;
		if (i == 10)
			shell->running = false;
	}
	ft_clear_memory(shell);
	//ft_success("Nice 🧡", 1);
	return (0);
}
