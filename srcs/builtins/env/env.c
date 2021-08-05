#include "../../../includes/minishell.h"

void	*ft_getenv(t_shell *shell, char *key, t_bool ret_struct)
{
	t_envp	*tmp;
	t_env	*env;

	tmp = shell->envp_2;
	env = NULL;
	while (tmp)
	{
		if (ft_strcmp(tmp->content->key, key) == 0)
		{
			env = tmp->content;
			break ;
		}
		tmp = tmp->next;
	}
	if (ret_struct)
		return (env);
	if (env)
		return (env->val);
	return (NULL);
}

void	ft_env(t_shell *shell, char **args)
{
	t_envp	*tmp;
	char	*res;
	char	*res2;

	if (args[1])
	{
		sh_error(shell, "minishell: env: too many arguments\n", 1);
		return ;
	}
	tmp = shell->envp_2;
	while (tmp)
	{
		if (tmp->content->has_val)
		{
			res = ft_strjoin(tmp->content->key, "=");
			res2 = ft_strjoin(res, tmp->content->val);
			ft_printf("%s\n", res2);
			ft_strdel(&res);
			ft_strdel(&res2);
		}
		tmp = tmp->next;
	}
}
