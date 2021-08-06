#include "../../../includes/minishell.h"

void	ft_print_declare(t_shell *shell)
{
	t_envp	*tmp;

	tmp = shell->envp_2;
	while (tmp)
	{
		ft_printf("declare -x %s", tmp->content->key);
		if (tmp->content->has_val)
			ft_printf("=\"%s\"", tmp->content->val);
		ft_printf("\n");
		tmp = tmp->next;
	}
}

static	void	ft_export_internal_sec(t_shell *shell, char *env, char *value)
{
	ft_envlstadd_back(&shell->envp_2, ft_envlstnew(ft_parse_env(env, value)));
}

/*
* Set an environment variable
*/
void	ft_export_internal(t_shell *shell, char *env, char *value)
{
	t_bool	skip;
	t_envp	*tmp;

	if (ft_isdigit(env[0]) || !(ft_isalnum(env[0]) || env[0] == '_'))
		return (sh_error(shell, ERR_EXI, 1, env));
	skip = (ft_strcmp(env, "HOME") == 0);
	tmp = shell->envp_2;
	while (tmp)
	{
		if (ft_strcmp(tmp->content->key, env) == 0)
		{
			if (skip)
				ft_cd_internal(shell, value, true);
			ft_strdel(&tmp->content->key);
			ft_strdel(&tmp->content->val);
			tmp->content->key = ft_strdup(env);
			tmp->content->val = ft_strdup(value);
			tmp->content->has_val = value[0] != '\0';
			return ;
		}
		tmp = tmp->next;
	}
	if (!skip)
		return (ft_export_internal_sec(shell, env, value));
}

static	t_bool	valid_export_init(t_shell *shell, char *s,
	char **val, size_t *i)
{
	t_bool	status;

	if (!*val)
		*val = ft_strdup("");
	if (ft_isdigit(s[0]) || !(ft_isalnum(s[0]) || s[0] == '_' || s[0] == '\\'))
	{
		if (*val[0] == DEL)
			sh_error(shell, ERR_EXI, 1, s);
		else
			sh_error(shell, ERR_EX, 1, s, *val);
		return (false);
	}
	status = true;
	(*i) = 0;
	return (status);
}

t_bool	valid_export(t_shell *shell, char *s, char **val)
{
	size_t	i;
	char	*bef;
	t_bool	status;

	status = valid_export_init(shell, s, val, &i);
	while (++i < ft_strlen(s))
	{
		if (!ft_isalnum(s[i]) && s[i] != '_' && s[i])
		{
			if (s[i + 1] == '\0' && s[i] == '+')
			{
				s[i] = '\0';
				bef = ft_strdup(ft_getenv(shell, s, false));
				if (!bef)
					bef = ft_strdup("");
				*val = ft_strjoin_free(bef, *val);
				break ;
			}
			sh_error(shell, ERR_EX, 1, s, *val);
			ft_strdel(val);
			return (false);
		}
	}
	return (status);
}
