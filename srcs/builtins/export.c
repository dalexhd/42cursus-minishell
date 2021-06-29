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
			shell->envp[i] = ft_strjoin_free(ft_strjoin(env, "="),
					ft_strdup(value));
			return ;
		}
		i++;
	}
	shell->envp[i] = ft_strjoin_free(ft_strjoin(env, "="), ft_strdup(value));
	shell->envp[i + 1] = NULL;
}

t_bool	valid_export(t_shell *shell, char *str, char **val)
{
	size_t	i;
	t_bool	status;
	char	*bef;

	if (ft_isdigit(str[0]) || !(ft_isalnum(str[0]) || str[0] == '_'))
	{
		ft_error("minishell: export: `%s=%s': not a valid identifier\n",
			false, str, val);
		if (str[0] == '+')
			shell->status = 1;
		else
			shell->status = 2;
		return (false);
	}
	status = true;
	i = 1;
	while (i < ft_strlen(str))
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
		{
			if (str[i + 1] == '\0' && str[i] == '+')
			{
				str[i] = '\0';
				bef = ft_getenv(shell, str);
				if (!bef)
					bef = ft_strdup("");
				*val = ft_strjoin(bef, *val);
				break ;
			}
			status = false;
			ft_error("minishell: export: `%s=%s': not a valid identifier\n",
				false, str, val);
			shell->status = 1;
			break ;
		}
		i++;
	}
	return (status);
}

void	ft_export(t_shell *shell, char **args)
{
	t_list	*tokens;
	char	*env;
	char	*value;
	size_t	i;

	if (!args[1])
		return ;
	else
	{
		i = 1;
		while (args[i])
		{
			tokens = ft_safesplitlist(args[i], '=', "\"'");
			env = ft_strdup(tokens->content);
			if (!tokens->next)
			{
				break ;
			}
			else if (!valid_export(shell, env, (char **)&tokens->next->content))
				break ;
			else if (tokens->next)
			{
				value = ft_strdup(tokens->next->content);
				ft_export_internal(shell, env, value);
			}
			i++;
		}
	}
}
