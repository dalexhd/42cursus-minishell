#include "includes/minishell.h"
#include <stdio.h>

void	ft_clear_memory(t_shell *shell)
{
	int	i;

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
	t_shell *shell;
	char *line;

	(void)argc;
	(void)**argv;
	shell = init_shell(envp);
	signal_handler();
	while (g_running)
	{
		if (shell->first)
			ft_fprintf(STDOUT_FILENO, C_CYAN "Wellcome to our minishell 😋" C_GREEN "❯ " C_X);
		else
			ft_fprintf(STDOUT_FILENO, C_GREEN "%s "C_GREEN "❯ " C_X, getCurrentDir(ft_pwd()));
		get_next_line(0, &line);
		if (line[0] != '\0')
			exec_shell(shell, line);
		free(line);
	}
	ft_clear_memory(shell);
	return (0);
}
