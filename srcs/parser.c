#include "../includes/minishell.h"

t_alist	*parse_args(t_shell *shell, char *cmd)
{
	t_alist	*args;
	t_args	*arg;
	int		argbackstatus;
	t_list	*tmp;
	t_list	*tmplist;
	int		fl;
	int		i;
	char	*new;
	char	*tmpchar;

	args = NULL;
	argbackstatus = 0;
	tmpchar = fix_cmd(cmd);
	if (validate_str(shell, tmpchar))
	{
		tmp = ft_safesplitlist(tmpchar, ' ', "\"'");
		//free(tmpchar);
		tmplist = tmp;
		char *aux;
		while (tmp)
		{
			arg = (t_args *)malloc(sizeof(t_args));
			aux = ft_strdup(tmp->content);
			arg->cmd = aux;
			//arg->cmd = ft_strdup(tmp->content);
			arg->file = NULL;
			arg->is_builtin = ft_isbuiltin(arg->cmd);
			arg->bin_path = NULL;
			arg->is_literal = 0;
			if (!arg->is_builtin)
				arg->bin_path = builtin_bin_path(shell, ft_strdup(arg->cmd));
			arg->type = 0;
			if (ft_strcmp(arg->cmd, ">>") == 0)
				arg->type = R_AOUT;
			else if (ft_strcmp(arg->cmd, ">") == 0)
				arg->type = R_OUT;
			else if (ft_strcmp(arg->cmd, "<") == 0)
				arg->type = R_IN;
			else if (ft_strstr(arg->cmd, "-"))
				arg->type = FLAG;
			else if (argbackstatus && (argbackstatus == R_IN || argbackstatus == R_OUT
					|| argbackstatus == R_AOUT
				))
			{
				arg->type = FILE;
				arg->file = arg->cmd;
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
			argbackstatus = arg->type;
			ft_alstadd_back(&args, ( shell->mierdecilla = ft_alstnew(arg) ));
			tmp = tmp->next;
		}
		ft_lstclear(&tmplist, free);
	}
	return (args);
}
/*
static char	*partial_trim(char *tmp, char *cmd, int i, int j)
{
	if (i != 0)
		tmp = ft_strcut(cmd, 0, i);
	else
		tmp = ft_strdup("");
	tmp = ft_strcat(tmp, ft_strcut(cmd, i + 1, j + 1));
	if (j != 0)
		tmp = ft_strcat(tmp, ft_strcut(cmd, j,	ft_strlen(cmd) - 1));
	return (tmp);
}
*/
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
