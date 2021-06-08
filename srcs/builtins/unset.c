#include "../includes/minishell.h"

/*
* Unset an environment variable
*/
void	ft_unset(t_shell *shell, char **args)
{
	int	i;
	int	u;

	i = 0;
	u = 0;
	if (!args[1])
	{
		ft_printf("Unset de mierda");
		return ;
	}
	while (shell->envp[i] != 0)
	{
		if (ft_strncmp(shell->envp[i], args[1], ft_strlen(args[1])) != 0)
		{
			ft_printf("%s\n", shell->envp[i]);
			shell->envp[u] = shell->envp[i];
			u++;
		}
		i++;
	}
}
