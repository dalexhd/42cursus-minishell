#include "../includes/minishell.h"

static	void	parse_type(t_shell *shell, t_alist *args, t_args *arg)
{
	int		fl;
	int		i;
	char	*new;

	arg->type = 0;
	if (ft_strcmp(arg->cmd, ">>") == 0)
		arg->type = R_AOUT;
	else if (ft_strcmp(arg->cmd, ">") == 0)
		arg->type = R_OUT;
	else if (ft_strcmp(arg->cmd, "<") == 0)
		arg->type = R_IN;
	else if (ft_strstr(arg->cmd, "-"))
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
		arg->cmd = parse_line(shell, arg, arg->cmd);
		new = ft_strnew(ft_strlen(arg->cmd));
		fl = 0;
		i = 0;
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
		ft_strdel(&new);
	}
}

static	t_args	*parse_arg(t_shell *shell, t_alist *args, t_list *value)
{
	t_args		*arg;

	arg = (t_args *)malloc(sizeof(t_args));
	arg->cmd = ft_strdup(value->content);
	arg->readable = true;
	arg->file = NULL;
	arg->bin_path = NULL;
	arg->is_builtin = ft_isbuiltin(arg->cmd);
	arg->is_literal = false;
	arg->redirect = (t_redirect *)malloc(sizeof(t_redirect));
	arg->redirect->in = (t_rstatus){.fd = 0, .file = NULL, .status = false};
	arg->redirect->out = (t_rstatus){.fd = 1, .file = NULL, .status = false};
	arg->redirect->aout = (t_rstatus){.fd = 0, .file = NULL, .status = false};
	if (!arg->is_builtin)
		arg->bin_path = builtin_bin_path(shell, arg->cmd);
	parse_type(shell, args, arg);
	return (arg);
}

void	parse_args(t_shell *shell, t_parsed **parsed, char *cmd)
{
	t_args	*arg;
	t_list	*tmp;
	t_list	*tmplist;
	char	*tmpchar;

	(*parsed)->args = NULL;
	tmpchar = fix_cmd(cmd);
	if (validate_str(shell, tmpchar))
	{
		tmp = ft_safesplitlist(tmpchar, ' ', "\"'");
		tmplist = tmp;
		while (tmp)
		{
			arg = parse_arg(shell, (*parsed)->args, tmp);
			ft_alstadd_back(&(*parsed)->args, ft_alstnew(arg));
			tmp = tmp->next;
		}
		ft_lstclear(&tmplist, free);
	}
}

char	*quotes_trim(char *cmd)
{
	int		i;
	char	out;
	t_bool	flag;

	i = 0;
	out = 0;
	while (i < (int)ft_strlen(cmd))
	{
		flag = true;
		if (i > 0 && cmd[i - 1] == '\\')
			flag = false;
		if (/*(cmd[i] != '\\') &&  */flag && (cmd[i] == '\'' || cmd[i] == '"'))
		{
			out = cmd[i];
			cmd[i] = DEL;
			if (out == '"')
			{
				while (cmd[i])
				{
					if (cmd[i] == out && cmd[i - 1] != '\\')
						break ;
					i++;
				}
				if (cmd[i] == out)
					cmd[i] = DEL;
			}
			else if (out == '\'')
			{
				cmd[i++] = DEL;
				while (cmd[i])
				{
					flag = true;
					if (i > 0 && cmd[i - 1] == '\\')
						flag = false;
					if (cmd[i] == out && cmd[i - 1] != '\\')
						break ;
					else if (!flag && cmd[i] != out && cmd[i] != '"')
					{
						cmd = ft_strdup(ft_strjoin(ft_strjoin(ft_strduplen(cmd, i - 1), "\\\\"), cmd + i));
						i += 2;
					}
					i++;
				}
				if (cmd[i] == out)
					cmd[i] = DEL;
			}
		}
		out = 0;
		i++;
	}
	i = 0;
	int j = 0;
	char *tmp = ft_strdup(cmd);
	while (i < (int)ft_strlen(cmd))
	{
		if (cmd[i] != DEL)
		{
			tmp[j] = cmd[i];
			j++;
		}
		i++;
	}
	tmp[j] = 0;
	free(cmd);
	cmd = ft_strdup(tmp);
	return (tmp);
}

char	*parse_line(t_shell *shell, t_args *arg, char *cmd)
{
	if (cmd)
		return (clean_str(shell, arg, ft_strdup(quotes_trim(cmd))));
	return (cmd);
}
