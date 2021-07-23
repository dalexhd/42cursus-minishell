#include "../includes/minishell.h"

t_bool	valid_unset(t_shell *shell, char *val)
{
	size_t	i;

	if (ft_isdigit(val[0]) || !(ft_isalnum(val[0]) || val[0] == '_'))
	{
		sh_error(shell, ERR_UN, (val[0] != '+') + 1);
		return (false);
	}
	i = 1;
	while (i < ft_strlen(val))
	{
		if (!(ft_isalnum(val[i]) || val[i] == '_'))
		{
			sh_error(shell, ERR_UN, 1, val);
			return (false);
		}
		i++;
	}
	return (true);
}

static void	inter_internal(t_shell *shell, char ***tmp, char *arg)
{
	char	*tmp2;
	int		i;
	int		u;

	i = 0;
	u = 0;
	while (shell->envp[i] != 0)
	{
		tmp2 = ft_strjoin(arg, "=");
		if (ft_strncmp(shell->envp[i], tmp2, ft_strlen(arg) + 1) != 0)
			(*tmp)[u++] = ft_strdup(shell->envp[i]);
		ft_strdel(&tmp2);
		i++;
	}
}

static	void	ft_internal_unset(t_shell *shell, char *arg)
{
	int		i;
	char	**tmp;

	i = 0;
	if (!arg || !valid_unset(shell, arg))
		return ;
	while (shell->envp[i] != 0)
		i++;
	tmp = ft_calloc(i + 1, sizeof(char *));
	if (ft_strcmp(arg, "HOME") == 0)
		shell->home_dir = NULL;
	else
	{
		inter_internal(shell, &tmp, arg);
		ft_split_del(shell->envp);
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
