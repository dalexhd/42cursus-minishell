#include "../../../includes/minishell.h"

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

static	t_bool	ft_export_declare(t_shell *shell, char **args,
	char **value, size_t *i)
{
	t_bool		status;

	*i = 0;
	*value = NULL;
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
	t_aslist	*t;
	char		*v;
	t_bool		has_equal;
	size_t		i;

	if (ft_export_declare(shell, args, &v, &i))
		return ;
	while (args[++i])
	{
		has_equal = ft_strchr(args[i], '=') != NULL;
		t = ret_tokens(args[i]);
		if (!t)
		{
			sh_error(shell, ERR_EXI, 1, "=");
			return (ft_aslstclear(&t, free));
		}
		if (!t->next || !t->next->content->arg)
			v = "\0";
		else
			v = t->next->content->arg;
		if (has_equal && v[0] == '\0')
			v = "\177\0";
		ft_export_sec(shell, ft_strdup(t->content->arg), ft_strdup(v), t);
	}
}
