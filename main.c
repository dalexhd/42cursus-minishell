#include "includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	t_list	*commands;

	shell = init_shell(envp);
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
		while (commands)
		{
			exec_shell(shell, commands->content);
			commands = commands->next;
		}
	}
	return (0);
}
