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
	char	*tmp;

	tmp = ft_strjoin(env, "=");
	i = 0;
	while (shell->envp[i] != 0)
	{
		if (ft_strncmp(shell->envp[i], tmp, ft_strlen(tmp)) == 0)
		{
			ft_strdel(&tmp);
			return (&shell->envp[i][ft_strlen(env) + 1]);
		}
		i++;
	}
	ft_strdel(&tmp);
	return (NULL);
}
