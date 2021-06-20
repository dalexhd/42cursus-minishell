#include "../includes/minishell.h"

static void replace_scape(char *cmd, size_t start, size_t count)
{
	while (start < ft_strlen(cmd) && count > 0)
	{
		if (cmd[start] == '\\' && count % 2)
			cmd[start] = DEL;
		count--;
		start++;
	}
}

static void	fix_scape(char *cmd)
{
	size_t	i;
	size_t	start;
	size_t	count;
	int		fl;
	char	*chr;

	fl = 0;
	i = 0;
	while (i < ft_strlen(cmd))
	{
		chr = ft_strchr("\"'", cmd[i]);
		if (chr && (!fl || fl & 1 << (chr - "\"'")))
			fl ^= 1 << (chr - "\"'");
		if (!fl)
		{
			if (cmd[i] == '\\')
			{
				count = 1;
				start = i;
				while (cmd[i + 1] == '\\')
				{
					count++;
					i++;
				}
				if (count % 2 == 1)
				{
					if (start + count + 1 < ft_strlen(cmd)
						&& !ft_strchr("\"'", cmd[start + count + 1]))
						fl = 1;
					replace_scape(cmd, start, count);
				}
			}
		}
		i++;
	}
}

static	char	*fix_cmd(char *cmd)
{
	size_t	i;
	char	*chr;
	int		fl;

	i = 0;
	fl = 0;
	fix_scape(cmd);
	while (i < ft_strlen(cmd))
	{
		chr = ft_strchr("\"'", cmd[i]);
		if (chr && (!fl || fl & 1 << (chr - "\"'")))
			fl ^= 1 << (chr - "\"'");
		if (!fl && (cmd[i] == '<' || cmd[i] == '>'))
		{
			if (cmd[i - 1] != '<' && cmd[i - 1] != '>' && cmd[i - 1] != ' '
				&& cmd[i - 1] != '\\')
			{
				cmd = ft_strduplen(cmd, ft_strlen(cmd) + 1);
				ft_insertchar(cmd, ' ', i);
				i++;
			}
			if (cmd[i + 1] != '<' && cmd[i + 1] != '>' && cmd[i + 1] != ' '
				&& cmd[i - 1] != '\\')
			{
				cmd = ft_strduplen(cmd, ft_strlen(cmd) + 1);
				ft_insertchar(cmd, ' ', i + 1);
				i++;
			}
		}
		i++;
	}
	return (cmd);
}

t_alist	*parse_args(t_shell *shell, char *cmd)
{
	t_alist		*args;
	t_args		*arg;
	t_args		*argback;
	t_list		*tmp;

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
			arg->is_literal = arg->cmd[0] == DEL || (arg->cmd[0] == '\''
				&& arg->cmd[ft_strlen(arg->cmd) - 1] == '\'');
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
				char *new = ft_strnew(ft_strlen(arg->cmd));
				int fl = 0;
				int i = 0;
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

char	*quotes_trim(char *cmd)
{
	int		i;
	char	one;
	int		fl;
	char	*tmp;
	char	*chr;
	int		j;

	i = 0;
	j = 0;
	one = 0;
	fl = 0;

	char del[2];
	del[0] = DEL;
	del[1] = '\0';
	while (cmd[i])
	{
		chr = ft_strchr(del, cmd[i]);
		if (chr && (!fl || fl & 1 << (chr - "\"'")))
			fl ^= 1 << (chr - "\"'");
		if (!fl && (cmd[i] == '\'' || cmd[i] == '"') && cmd[i])
		{
			one = cmd[i];
			tmp = ft_strrev(ft_strdup(cmd));
			while (tmp[j] != one)
				j++;
			if (tmp[j] == one && i > 0 && cmd[i - 1] != DEL)
			{
				cmd[i] = DEL;
				cmd[j] = DEL;
				return (quotes_trim(cmd));
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
