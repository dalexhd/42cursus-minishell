#include "../includes/minishell.h"

t_bool	valid_unset(t_shell *shell, char *val)
{
	size_t	i;

	if (ft_isdigit(val[0]) || !(ft_isalnum(val[0]) || val[0] == '_'))
	{
		sh_error(shell, "minishell: unset: `%s': not a valid identifier\n", (val[0] != '+') + 1);
		return (false);
	}
	i = 1;
	while (i < ft_strlen(val))
	{
		if (!(ft_isalnum(val[i]) || val[i] == '_'))
		{
			sh_error(shell, "minishell: unset: `%s': not a valid identifier\n", 1, val);
			return (false);
		}
		i++;
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
	if (!arg || !valid_unset(shell, arg))
		return ;
	while (shell->envp[i] != 0)
		i++;
	tmp = calloc(i + 1, sizeof(char *));
	i = 0;
	if (ft_strcmp(arg, "HOME") == 0)
		shell->home_dir = NULL;
	else
	{
		while (shell->envp[i] != 0)
		{
			if (ft_strncmp(shell->envp[i], ft_strjoin(arg, "="),
					ft_strlen(arg) + 1) != 0)
				tmp[u++] = shell->envp[i];
			i++;
		}
		shell->envp = tmp;
	}
}

/*
** Unset an environment variable
*/
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
