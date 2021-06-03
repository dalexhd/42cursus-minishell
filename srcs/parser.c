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

t_list	*parse_args(char *cmd)
{
	t_list		*args;

	args = NULL;
	if (valid_quotes(cmd))
	{
		args = ft_safesplitlist(cmd, ' ', "\"'");
	}
	return (args);
}

char	*parse_line(t_shell *shell, char *cmd)
{
	if (cmd)
		return (clean_str(shell, ft_strdup(ft_strtrim(cmd, "\""))));
	return (cmd);
}
