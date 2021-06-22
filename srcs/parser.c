#include "../includes/minishell.h"

t_alist	*parse_args(t_shell *shell, char *cmd)
{
	t_alist	*args;
	t_args	*arg;
	t_args	*argback;
	t_list	*tmp;
	int		fl;
	int		i;
	char	*new;

	args = NULL;
	argback = NULL;
	cmd = fix_cmd(cmd);
	if (validate_str(shell, cmd))
	{
		tmp = ft_safesplitlist(cmd, ' ', "\"'");
		while (tmp)
		{
			arg = (t_args *)malloc(sizeof(t_args));
			arg->cmd = tmp->content;
			arg->file = NULL;
			arg->is_builtin = ft_isbuiltin(tmp->content);
			arg->bin_path = NULL;
			arg->is_literal = arg->cmd[0] == '\''
				&& arg->cmd[ft_strlen(arg->cmd) - 1] == '\'';
			if (!arg->is_builtin)
				arg->bin_path = builtin_bin_path(shell, tmp->content);
			arg->type = 0;
			if (ft_strcmp(tmp->content, ">>") == 0)
				arg->type = R_AOUT;
			else if (ft_strcmp(tmp->content, ">") == 0)
				arg->type = R_OUT;
			else if (ft_strcmp(tmp->content, "<") == 0)
				arg->type = R_IN;
			else if (ft_strstr(tmp->content, "-"))
				arg->type = FLAG;
			else if (argback && (argback->type == R_IN || argback->type == R_OUT
					|| argback->type == R_AOUT
				))
			{
				arg->type = FILE;
				arg->file = tmp->content;
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
				arg->cmd = new;
				arg->bin_path = arg->cmd;
				arg->type = ARG;
			}
			argback = arg;
			ft_alstadd_back(&args, ft_alstnew(arg));
			tmp = tmp->next;
		}
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
	char	*tmp;
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
				tmp = ft_strdup(cmd);
				while (tmp[i])
				{
					if (tmp[i] == out && tmp[i - 1] != '\\')
						break ;
					i++;
				}
				if (tmp[i] == out)
					cmd[i] = DEL;
			}
			else if (out == '\'')
			{
				tmp = ft_strdup(cmd + i);
			}
		}
		i++;
	}

	return (cmd);
}

char	*parse_line(t_shell *shell, t_args *arg, char *cmd)
{
	if (cmd)
		return (clean_str(shell, arg, ft_strdup(quotes_trim(cmd))));
	return (cmd);
}
