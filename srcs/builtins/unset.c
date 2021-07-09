#include "../includes/minishell.h"

/*
* Unset an environment variable
*/

t_bool	valid_unset(t_shell *shell, char *val)
{
	if (ft_isdigit(val[0]) || !(ft_isalnum(val[0]) || val[0] == '_'))
	{
		ft_error("minishell: unset: `%s': not a valid identifier\n",
			false, val);
		if (val[0] == '+')
			shell->status = 1;
		else
			shell->status = 2;
		return (false);
	}
	return (true);
}

static	void	ft_internal_unset(t_shell *shell, char *arg)
{
	int		i;
	int		u;
	char	**tmp;

	i = 0;
	u = 0;
	if (!arg)
		return ;
	while (shell->envp[i] != 0)
		i++;
	tmp = (char **)malloc(sizeof(char *) * (i) + 1);
	i = 0;
	if (ft_strcmp(arg, "HOME") == 0)
		shell->home_dir = NULL;
	while (shell->envp[i] != 0)
	{
		if (ft_strncmp(shell->envp[i], ft_strjoin(arg, "="),
				ft_strlen(arg) + 1) != 0)
		{
			tmp[u] = shell->envp[i];
			u++;
		}
		i++;
	}
	shell->envp = tmp;
	shell->envp[u] = NULL;
}

void	ft_unset(t_shell *shell, char **args)
{
	int		e;

	e = 1;
	while (args[e])
	{
		shell->status = 0;
		ft_internal_unset(shell, args[e]);
		e++;
	}
}

