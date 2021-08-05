#include "../../../includes/minishell.h"

t_env	*ft_parse_env(char *key, char *val)
{
	t_env	*env;
	char	*ret_key;
	char	*ret_val;
	t_bool	ret_has;

	ret_key = key;
	ret_val = val;
	ret_has = true;
	env = (t_env *)malloc(sizeof(t_env));
	if (val[0] == '\0')
		ret_has = false;
	if (val[0] == DEL)
		ret_val[0] = '\0';
	env->key = ft_strdup(ret_key);
	env->val = ft_strdup(ret_val);
	env->has_val = ret_has;
	return (env);
}
