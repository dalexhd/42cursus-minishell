#include "../includes/minishell.h"

/*
* Set an environment variable
*/
void	ft_export_internal(t_shell *shell, char *env, char *value)
{
	int		i;
	char	*tmp;
	t_bool	skip;

	i = 0;
	skip = false;
	if (ft_isdigit(env[0]) || !(ft_isalnum(env[0]) || env[0] == '_'))
	{
		sh_error(shell, "minishell: export: `%s': not a valid identifier\n",
			1, env);
		return ;
	}
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], env, ft_strlen(env)) == 0)
		{
			if (ft_strcmp(env, "HOME") == 0)
			{
				skip = true;
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
	{
		shell->envp = ft_realloc(shell->envp, sizeof(char *) * (ft_splitlen(shell->envp) + 2));
		tmp = ft_strjoin_free(ft_strjoin(env, "="), ft_strdup(value));
		ft_strdel(&shell->envp[i]);
		shell->envp[i] = tmp;
		shell->envp[i + 1] = NULL;
	}
}

t_bool	valid_export(t_shell *shell, char *str, char **val)
{
	size_t	i;
	t_bool	status;
	char	*bef;

	if (ft_isdigit(str[0]) || !(ft_isalnum(str[0]) || str[0] == '_' || str[0] == '\\'))
	{
		sh_error(shell, "minishell: export: `%s=%s': not a valid identifier\n",
			(str[0] != '+') + 1, str, *val);
		return (false);
	}
	status = true;
	i = 1;
	while (i < ft_strlen(str))
	{
		if (!ft_isalnum(str[i]) && str[i] != '_' && str[i])
		{
			if (str[i + 1] == '\0' && str[i] == '+')
			{
				str[i] = '\0';
				bef = ft_strdup(ft_getenv(shell, str));
				if (!bef)
					bef = ft_strdup("");
				*val = ft_strjoin_free(bef, *val);
				break ;
			}
			status = false;
			sh_error(shell, "minishell: export: `%s=%s': not a valid identifier\n",
				1, str, *val);
			break ;
		}
		i++;
	}
	return (status);
}

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

void	ft_export(t_shell *shell, char **args)
{
	t_aslist	*tokens;
	t_aslist	*tokens_tmp;
	char		*env;
	char		*value;
	size_t		i;

	if (!args[1])
	{
		ft_print_declare(shell);
		return ;
	}
	else
	{
		i = 1;
		while (args[i])
		{
			tokens = ft_safesplitlist(args[i], '=', "\"'", false);
			if (!tokens)
			{
				sh_error(shell, "minishell: export: `=': not a valid identifier\n", 1);
				ft_aslstclear(&tokens, free);
				break ;
			}
			tokens_tmp = tokens;
			env = ft_strdup(tokens->content->arg);
			value = NULL;
			if (!tokens->next)
			{
				if (valid_export(shell, env, &value))
					ft_export_internal(shell, env, "");
			}
			else
			{
				value = ft_strdup(tokens->next->content->arg);
				if (!valid_export(shell, env, &value))
				{
					ft_strdel(&env);
					ft_strdel(&value);
					ft_aslstclear(&tokens_tmp, free);
					break ;
				}
				else
				{
					ft_export_internal(shell, env, value);
					shell->status = 0;
				}
			}
			ft_strdel(&env);
			if (value)
				ft_strdel(&value);
			ft_aslstclear(&tokens_tmp, free);
			i++;
		}
	}
}
