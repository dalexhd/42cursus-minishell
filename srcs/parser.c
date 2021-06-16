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
		if (cmd[i] == '<' || cmd[i] == '>')
		{
			two = cmd[i];
			if (cmd[i] == '>' && cmd[i + 1] == '>' && cmd[i + 1])
				i++;
			pos = i++;
			while (cmd[i] == ' ')
				i++;
			if (cmd[i] != '>' && cmd[i] != '<')
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

static char	*addLetter(char *s, char c, size_t pos)
{
	size_t i;

	i = 0;
	while (i < pos && s[i])
		i++;
	if (i == pos)
	{
		while (c)
		{
			char tmp = s[i];
			s[i] = c;
			c = tmp;
			i++;
		}
		s[i] = c;
	}

	return s;
}

static	char *ft_strdupcus(const char *src, size_t len) {
	len = len + 1;       // String plus '\0'
	char *dst = (char *) malloc(sizeof(char) * len);            // Allocate space
	if (dst == NULL) return NULL;       // No memory
	ft_memcpy(dst, src, len);             // Copy the block
	return dst;                         // Return the new string
}

static	char	*fix_cmd(char *cmd)
{
	int			i;
	int			end;
	int			safequotes[2];
	char		*cut;

	i = 0;
	safequotes[0] = 1;
	safequotes[1] = 1;
	while (i < ft_strlen(cmd))
	{
		if (cmd[i] == '"')
			safequotes[0] = 0;
		if (safequotes[0] && cmd[i] == '\'')
			safequotes[1] = 0;
		if (safequotes[0] && safequotes[1] && (cmd[i] == '>' || cmd[i] == '<'))
		{
			if (i > 0 && cmd[i - 1] != ' ' && cmd[i - 1] != '>' && cmd[i - 1] != '<')
			{
				cmd = ft_strdupcus(cmd, ft_strlen(cmd) + 1);
				addLetter(cmd, ' ', i);
				i++;
			}
		}
		else if (i > 0 && (cmd[i - 1] == '>' || cmd[i - 1] == '<')
			&& (cmd[i] != '>' || cmd[i] != '<')
			&& cmd[i] != ' '
		)
		{
			cmd = ft_strdupcus(cmd, ft_strlen(cmd) + 1);
			addLetter(cmd, ' ', i);
			i++;
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
