#include "../includes/minishell.h"

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	g_running = true;
	shell->should_wait = true;
	shell->envp = envp;
	shell->parsed = NULL;
	shell->first = true;
	shell->is_cmd = false;
	shell->status = 0;
	return (shell);
}

void	exec_shell(t_shell *shell, char *cmd)
{
	if (ft_strlen(cmd) > 0)
	{
		ft_printf("Enter!\n");
		shell->parsed = NULL;
		shell->first = false;
		lsh_split_line(shell, cmd);
		shell->pipe_count = ft_slstsize(shell->parsed);
		fill_data(shell->parsed);
		if (shell->status > -1)
			run(shell);
	}
}
