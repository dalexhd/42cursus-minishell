#include "../includes/minishell.h"

/*
* Unset an environment variable
*/

void	ft_unset(t_shell *shell, char **args)
{
	int		i;
	int		u;
	char	**tmp;

	i = 0;
	u = 0;
	if (!args[1])
		return ;
	while (shell->envp[i] != 0)
		i++;
	tmp = (char **)malloc(sizeof(char *) * (i) + 1);
	i = 0;
	while (shell->envp[i] != 0)
	{
		if (ft_strncmp(shell->envp[i], ft_strjoin(args[1], "="), ft_strlen(args[1]) + 1) != 0)
		{
			tmp[u] = shell->envp[i];
			u++;
		}
		i++;
	}
	*shell->envp = NULL;
	shell->envp = tmp;
	shell->envp[u] = NULL;
}
