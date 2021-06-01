#include "../includes/minishell.h"

void	ft_env(t_shell *shell)
{
	int		i;

	i = 0;
	while (shell->envp[i] != 0)
	{
		ft_printf("%s\n", shell->envp[i]);
		i++;
	}
}

/*
* Get an environment variable
*/
char	*ft_getenv(t_shell *shell, char *env)
{
	int	i;

	i = 0;
	while (shell->envp[i] != 0)
	{
		if (ft_strncmp(shell->envp[i], env, ft_strlen(env)) == 0)
			return (ft_substr(shell->envp[i], ft_strlen(env) + 1,
					ft_strlen(shell->envp[i])));
		i++;
	}
	return (NULL);
}
