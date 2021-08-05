#include "../../includes/minishell.h"

t_bool	valid_unset(t_shell *shell, char *val)
{
	size_t	i;

	if (ft_isdigit(val[0]) || !(ft_isalnum(val[0]) || val[0] == '_'))
	{
		sh_error(shell, ERR_UN, !ft_strchr("+=", val[0]) + 1, val);
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

static	t_bool	filter_del(t_env *env, char *arg)
{
	return (ft_strcmp(env->key, arg) != 0);
}

static void	inter_internal(t_shell *shell, char *arg)
{
	shell->envp_2 = ft_envlstfilterarg(shell->envp_2, filter_del,
			del_envlst, arg);
}

static	void	ft_internal_unset(t_shell *shell, char *arg)
{
	if (!arg || !valid_unset(shell, arg))
		return ;
	if (ft_strcmp(arg, "HOME") == 0)
		shell->home_dir = NULL;
	else
		inter_internal(shell, arg);
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
