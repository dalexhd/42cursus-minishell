#include "../includes/minishell.h"

int	ft_echo(char **args)
{
	size_t	i;
	t_bool	skip_new_line;

	skip_new_line = false;
	i = 1;
	if (!args[1])
		ft_printf("\n");
	if (args[i] && !ft_strcmp(args[i], "-n"))
	{
		skip_new_line = true;
		while (args[i + 1] && !ft_strcmp(args[i + 1], "-n"))
			i++;
		args = &args[1];
	}
	while (args[i])
	{
		if (args[i + 1])
			ft_printf("%s ", args[i]);
		else if (skip_new_line)
			ft_printf("%s", args[i]);
		else
			ft_printf("%s\n", args[i]);
		i++;
	}
	return (0);
}
