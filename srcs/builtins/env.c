#include "../includes/minishell.h"

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
