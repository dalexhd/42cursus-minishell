#include "includes/minishell.h"

static int	shell_clear(t_shell *shell)
{
	int	status;

	status = shell->status;
	if (shell->tmp_dir)
		free(shell->tmp_dir);
	ft_hlstclear(&shell->term.history, free);
	free(shell);
	if (status != 0)
		exit(status);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	t_list	*commands;
	t_list	*commands_tmp;

	shell = init_shell(envp);
	signal_handler();
	if (argc != 3)
	{
		init_tc(shell);
		ft_printshell(shell);
		loureed(shell);
		end_tc(shell);
	}
	else
	{
		commands = ft_safesplitlist(argv[2], ';', "\"'");
		commands_tmp = commands;
		while (commands)
		{
			exec_shell(shell, commands->content);
			commands = commands->next;
		}
		ft_lstclear(&commands_tmp, free);
	}
	return (shell_clear(shell));
}
