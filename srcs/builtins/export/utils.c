#include "../includes/minishell.h"

void	ft_print_declare(t_shell *shell)
{
	int		i;
	char	*key;
	char	*value;

	i = 0;
	while (shell->envp[i])
	{
		key = ft_substr(shell->envp[i], 0,
				(int)(ft_strchr(shell->envp[i], '=') - shell->envp[i]));
		value = ft_strdup(ft_strstr(shell->envp[i], "="));
		ft_printf("declare -x %s=\"%s\"\n", key, ft_strcpy(&value[0], &value[1]));
		ft_strdel(&key);
		ft_strdel(&value);
		i++;
	}
}

static	void	ft_export_internal_sec(t_shell *shell, char *env, char *value,
	int i)
{
	char	*tmp;

	shell->envp = ft_realloc(shell->envp,
			sizeof(char *) * (ft_splitlen(shell->envp) + 2));
	tmp = ft_strjoin_free(ft_strjoin(env, "="), ft_strdup(value));
	ft_strdel(&shell->envp[i]);
	shell->envp[i] = tmp;
}

/*
* Set an environment variable
*/
void	ft_export_internal(t_shell *shell, char *env, char *value)
{
	int		i;
	t_bool	skip;

	i = 0;
	if (ft_isdigit(env[0]) || !(ft_isalnum(env[0]) || env[0] == '_'))
		return (sh_error(shell, ERR_EXI, 1, env));
	skip = (ft_strcmp(env, "HOME") == 0);
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], env, ft_strlen(env)) == 0)
		{
			if (skip)
			{
				ft_cd_internal(shell, value);
				shell->home_dir = ft_pwd();
			}
			ft_strdel(&shell->envp[i]);
			shell->envp[i] = ft_strjoin_free(ft_strjoin(env, "="),
					ft_strdup(value));
			return ;
		}
		i++;
	}
	if (!skip)
		return (ft_export_internal_sec(shell, env, value, i));
}

static	t_bool	valid_export_init(t_shell *shell, char *s,
	char **val, size_t *i)
{
	t_bool	status;

	if (ft_isdigit(s[0]) || !(ft_isalnum(s[0]) || s[0] == '_' || s[0] == '\\'))
	{
		sh_error(shell, ERR_EX, !ft_strchr("+=", s[0]) + 1, s, *val);
		return (false);
	}
	status = true;
	(*i) = 1;
	return (status);
}

t_bool	valid_export(t_shell *shell, char *s, char **val)
{
	size_t	i;
	char	*bef;
	t_bool	status;

	status = valid_export_init(shell, s, val, &i);
	while (i < ft_strlen(s))
	{
		if (!ft_isalnum(s[i]) && s[i] != '_' && s[i])
		{
			if (s[i + 1] == '\0' && s[i] == '+')
			{
				s[i] = '\0';
				bef = ft_strdup(ft_getenv(shell, s));
				if (!bef)
					bef = ft_strdup("");
				*val = ft_strjoin_free(bef, *val);
				break ;
			}
			status = false;
			sh_error(shell, ERR_EX, 1, s, *val);
			break ;
		}
		i++;
	}
	return (status);
}
