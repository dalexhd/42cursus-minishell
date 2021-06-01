
#include "../includes/minishell.h"

/*
* Set an environment variable
*/
void	ft_export_internal(t_shell *shell, char *env, char *value)
{
	int	i;

	i = 0;
	while (shell->envp[i] != 0)
	{
		if (ft_strncmp(shell->envp[i], env, ft_strlen(env)) == 0)
		{
			shell->envp[i] = ft_strjoin_free(ft_strjoin(env, "="), ft_strdup(value));
			return ;
		}
		i++;
	}
	shell->envp[i] = ft_strjoin_free(ft_strjoin(env, "="), ft_strdup(value));
	shell->envp[i + 1] = NULL;
}

void	ft_export(t_shell *shell, char **args)
{
	t_list	*tokens;
	char	*env;
	char	*value;
	size_t	i;

	if (!args[1])
		ft_printf("Here print declares!\n");
	else
	{
		i = 1;
		while (args[i])
		{
			tokens = ft_safesplitlist(args[i], '=', "\"'");
			env = ft_strdup(tokens->content);
			if (tokens->next)
				value = ft_strdup(tokens->next->content);
			else
				value = ft_strdup("");
			ft_export_internal(shell, env, value);
			i++;
		}
	}
}
