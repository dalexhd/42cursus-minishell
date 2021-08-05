#include "../../includes/minishell.h"

void	echoes(char ***args, t_bool *skip_new_line, size_t *i)
{
	int	u;

	if (!(*args)[1])
		ft_printf("\n");
	if ((*args)[*i] && !ft_strncmp((*args)[*i], "-n", 2))
	{
		u = 2;
		while (u < (int)ft_strlen((*args)[*i]))
		{
			if ((*args)[*i][u] != 'n')
			{
				u = -1;
				break ;
			}
			u++;
		}
		if (u != -1)
		{
			*skip_new_line = true;
			while ((*args)[*i + 1] && (!ft_strncmp((*args)[*i + 1], "-n", 2)
					&& (*args)[*i + 1][ft_strlen((*args)[*i + 1]) - 1] == 'n'))
				(*i)++;
			*args = &(*args)[1];
		}
	}
}

int	ft_echo(t_shell *shell, char **args)
{
	size_t	i;
	t_bool	skip_new_line;

	skip_new_line = false;
	i = 1;
	echoes(&args, &skip_new_line, &i);
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
	shell->status = 0;
	return (0);
}
