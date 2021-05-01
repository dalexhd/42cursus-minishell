#include "includes/minishell.h"

int	main(void)
{
	t_shell	shell;
	int		i;

	shell = init_shell();
	i = 0;
	while (shell.running)
	{
		i++;
		ft_printf("Hola mundo");
		if (i == 10)
			shell.running = false;
	}
	ft_success("Nice 🧡", 1);
	return (0);
}