#include "../includes/minishell.h"

static void	parse_typo_sec(t_args *arg, char *new)
{
	int		i;
	int		fl;

	fl = 0;
	i = 0;
	if (ft_strlen(arg->cmd) == 1 && arg->cmd[0] == DEL)
		return ;
	if (arg->cmd[0] == '\0')
		arg->readable = false;
	if (arg->cmd[i] == '\0')
		new[fl] = '\0';
	while (arg->cmd[i])
	{
		if (arg->cmd[i] != DEL)
		{
			new[fl] = arg->cmd[i];
			fl++;
		}
		i++;
	}
	ft_strdel(&arg->cmd);
	arg->cmd = ft_strdup(new);
	arg->bin_path = ft_strdup(new);
	arg->type = ARG;
}

static void	parse_typo(t_shell *shell, t_args *arg)
{
	char	*new;

	arg->cmd = parse_line(shell, arg, arg->cmd);
	arg->is_builtin = ft_isbuiltin(arg->cmd);
	if (!arg->is_builtin)
	{
		if (arg->bin_path)
			ft_strdel(&arg->bin_path);
		arg->bin_path = builtin_bin_path(shell, arg->cmd);
	}
	if (arg->is_builtin || arg->bin_path)
	{
		arg->type = CMD;
		return ;
	}
	new = ft_strnew(ft_strlen(arg->cmd));
	parse_typo_sec(arg, new);
	ft_strdel(&new);
}

static	void	parse_type(t_shell *shell, t_alist *args, t_args *arg)
{
	arg->type = 0;
	if (arg->quot_type == N_QUOT && ft_strcmp(arg->cmd, ">>") == 0)
		arg->type = R_AOUT;
	else if (arg->quot_type == N_QUOT && ft_strcmp(arg->cmd, ">") == 0)
		arg->type = R_OUT;
	else if (arg->quot_type == N_QUOT && ft_strcmp(arg->cmd, "<") == 0)
		arg->type = R_IN;
	else if (arg->cmd[1] == '-')
		arg->type = FLAG;
	else if (args && arg_has_red(ft_alstlast(args)))
	{
		arg->type = FILE;
		arg->file = ft_strdup(arg->cmd);
	}
	else if (arg->is_builtin || arg->bin_path)
		arg->type = CMD;
	else
	{
		parse_typo(shell, arg);
	}
}

t_args	*parse_arg(t_shell *shell, t_alist *args, t_aslist *value)
{
	t_args		*arg;

	arg = (t_args *)malloc(sizeof(t_args));
	arg->cmd = remove_cmd_quotes(ft_strdup(value->content->arg));
	arg->readable = true;
	arg->file = NULL;
	arg->bin_path = NULL;
	arg->is_builtin = ft_isbuiltin(arg->cmd);
	arg->is_literal = false;
	arg->quot_type = get_quote_type(value->content->arg);
	arg->spaced = !value->content->concat;
	arg->redirect = (t_redirect *)malloc(sizeof(t_redirect));
	arg->redirect->in = (t_rstatus){.fd = 0, .file = NULL, .status = false};
	arg->redirect->out = (t_rstatus){.fd = 1, .file = NULL, .status = false};
	arg->redirect->aout = (t_rstatus){.fd = 0, .file = NULL, .status = false};
	if (!arg->is_builtin)
		arg->bin_path = builtin_bin_path(shell, arg->cmd);
	parse_type(shell, args, arg);
	return (arg);
}
