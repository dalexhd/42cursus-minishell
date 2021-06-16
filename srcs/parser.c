#include "../includes/minishell.h"

static int	quotes(char *cmd, int i, char c)
{
	while (cmd[i] && cmd[i] != c)
		i++;
	if (!cmd[i])
		return (-42);
	return (i);
}

t_bool	tof(char *cmd, int pos, char one)
{
	if (one == '\'' || one == '"')
	{
		ft_error("minishell: Unmaching opening quotes at col %d: %s\n",
			0, pos, cmd + (pos));
		return (false);
	}
	else if (one == '<' || one == '>')
	{
		ft_error("minishell: syntax error near redirect: %s\n",
			0, cmd + (pos));
		return (false);
	}
	return (true);
}

t_bool	valid_quotes(char *cmd)
{
	int		i;
	char	one;
	char	two;
	int		pos;
	int		red;

	i = 0;
	one = 0;
	two = 0;
	while (cmd[i])
	{
		if ((cmd[i] == '<' || cmd[i] == '>') && !two)
		{
			two = cmd[i];
			if (cmd[i] == '>' && cmd[i + 1] == '>' && cmd[i + 1])
				i++;
			pos = i++;
			while (cmd[i + 1] == ' ')
				i++;
			if (ft_isalnum(cmd[i]))
				red = 0;
			else
				return (tof(cmd, pos, two));
		}
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
	return (tof(cmd, pos, one));
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
			{
				arg->cmd = parse_line(shell, arg, arg->cmd);
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

char	*quotes_trim(char *cmd)
{
	int		i;
	char	one;
	char	*tmp;
	int		j;

	i = 0;
	j = 0;
	one = 0;
	while (cmd[i])
	{
		if ((cmd[i] == '\'' || cmd[i] == '"') && cmd[i])
		{
			one = cmd[i];
			tmp = ft_strrev(ft_strdup(cmd));
			while (tmp[j] != one)
				j++;
			if (tmp[j] == one)
			{
				if (i != 0)
					tmp = ft_strcut(cmd, 0, i);
				else
					tmp = ft_strdup("");
				tmp = ft_strcat(tmp, ft_strcut(cmd, i + 1,
							ft_strlen(cmd) - j - 2));
				if (j != 0)
					tmp = ft_strcat(tmp, ft_strcut(cmd, ft_strlen(cmd) - j,
								ft_strlen(cmd)));
//				else
//					tmp = ft_strcat(tmp, ft_strcut(cmd, ft_strlen(cmd) - j, ft_strlen(cmd)));
		//			tmp = ft_strcat(tmp, "");
				return (tmp);
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
/*
//1. quita espacios
Comillas dobles
*/
