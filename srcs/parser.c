#include "../includes/minishell.h"

static int	quotes(char *cmd, int i, char c)
{
	while (cmd[i] && cmd[i] != c)
		i++;
	if (!cmd[i])
		return (-42);
	return (i);
}

t_bool	tof_quotes(char *cmd, int pos, char one)
{
	if (one)
	{
		ft_error("minishell: Unmaching opening quotes at col %d: %s\n",
			0, pos, cmd + (pos));
		return (false);
	}
	return (true);
}

t_bool	valid_quotes(char *cmd)
{
	int		i;
	char	one;
	int		pos;

	i = 0;
	one = 0;
	while (cmd[i])
	{
		if ((cmd[i] == '\'' || cmd[i] == '"') && cmd[i])
		{
			one = cmd[i];
			pos = i++;
			i = quotes(cmd, i, one);
			if (i == -42)
				break ;
			else if (cmd[i] == one)
				one = 0;
		}
		i++;
	}
	return (tof_quotes(cmd, pos, one));
}

t_alist	*parse_args(t_shell *shell, char *cmd)
{
	t_alist		*args;
	t_args		*arg;
	t_args		*argback;
	t_list		*tmp;

	args = NULL;
	argback = NULL;
	if (valid_quotes(cmd))
	{
		tmp = ft_safesplitlist(cmd, ' ', "\"'");
		while (tmp)
		{
			arg = (t_args *)malloc(sizeof(t_args));
			arg->cmd = tmp->content;
			arg->file = NULL;
			arg->is_builtin = ft_isbuiltin(tmp->content);
			arg->bin_path = NULL;
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
			else if (argback && (argback->type == R_IN
					|| argback->type == R_OUT
					|| argback->type == R_AOUT
				))
			{
				arg->type = FILE;
				arg->file = tmp->content;
			}
			else if (arg->is_builtin || arg->bin_path)
				arg->type = CMD;
			else
				arg->type = ARG;
			argback = arg;
			ft_alstadd_back(&args, ft_alstnew(arg));
			tmp = tmp->next;
		}
	}
	return (args);
}

char	*parse_line(t_shell *shell, char *cmd)
{
	if (cmd)
		return (clean_str(shell, ft_strdup(ft_strtrim(cmd, "\""))));
	return (cmd);
}
