#include "../includes/minishell.h"

static	void	ft_export_sec(t_shell *shell, char *env, char *value,
	t_aslist	*tokens_tmp)
{
	if (valid_export(shell, env, &value))
	{
		ft_export_internal(shell, env, value);
		shell->status = 0;
	}
	ft_strdel(&env);
	ft_strdel(&value);
	ft_aslstclear(&tokens_tmp, free);
}

static	t_bool	ft_export_declare(t_shell *shell, char **args)
{
	t_bool		status;

	status = false;
	if (!args[1])
	{
		status = true;
		ft_print_declare(shell);
	}
	return (status);
}

static	t_aslist	*ret_tokens(char *arg)
{
	t_aslist	*tokens;

	g_running = ';';
	tokens = ft_safesplitlist(arg, '=', "\"'");
	g_running = true;
	return (tokens);
}

void	ft_export(t_shell *shell, char **args)
{
	t_aslist	*tokens;
	t_aslist	*tokens_tmp;
	char		*env;
	char		*value;
	size_t		i;

	if (ft_export_declare(shell, args))
		return ;
	i = 1;
	while (args[i])
	{
		tokens = ret_tokens(args[i]);
		if (!tokens)
		{
			sh_error(shell, ERR_EXI, 1, "=");
			return (ft_aslstclear(&tokens, free));
		}
		tokens_tmp = tokens;
		if (!tokens->next)
			return (ft_aslstclear(&tokens_tmp, free));
		env = ft_strdup(tokens->content->arg);
		value = ft_strdup(tokens->next->content->arg);
		ft_export_sec(shell, env, value, tokens_tmp);
		i++;
	}
}
