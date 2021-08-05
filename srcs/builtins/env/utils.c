#include "../../../includes/minishell.h"

static	char	*ft_getenv_key(char *envp)
{
	size_t	i;
	char	*res;

	i = 0;
	res = NULL;
	while (i < ft_strlen(envp))
	{
		if (envp[i] == '=')
		{
			res = ft_strcut(envp, 0, i);
			break ;
		}
		i++;
	}
	return (res);
}

static	char	*ft_getenv_val(char *envp)
{
	size_t	i;
	char	*res;

	i = 0;
	res = NULL;
	while (i < ft_strlen(envp))
	{
		if (envp[i] == '=')
		{
			if (envp[i + 1] == '\0')
				res = ft_strdup("");
			else
				res = ft_strcut(envp, i + 1, ft_strlen(envp));
			break ;
		}
		i++;
	}
	return (res);
}

t_envp	*ft_setup_env(t_shell *shell, char **envp)
{
	t_envp	*envp_list;
	t_env	*env;
	size_t	i;

	envp_list = NULL;
	i = 0;
	while (envp[i])
	{
		env = (t_env *)malloc(sizeof(t_env));
		env->key = ft_getenv_key(envp[i]);
		env->val = ft_getenv_val(envp[i]);
		env->has_val = true;
		ft_envlstadd_back(&envp_list, ft_envlstnew(env));
		if (ft_strcmp(env->key, "HOME") == 0)
			shell->home_dir = env->val;
		i++;
	}
	return (envp_list);
}
