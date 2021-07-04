#include "includes/minishell.h"

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
		shell->is_cmd = true;
		commands = ft_safesplitlist(argv[2], ';', "\"'");
		commands_tmp = commands;
		while (commands)
		{
			exec_shell(shell, commands->content);
			commands = commands->next;
		}
		ft_lstclear(&commands_tmp, free);
	}
	if (shell->status != 0)
		exit(shell->status);
	return (0);
}
