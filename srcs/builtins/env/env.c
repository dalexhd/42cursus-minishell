#include "../includes/minishell.h"

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
