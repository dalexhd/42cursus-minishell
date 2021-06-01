#include "../includes/minishell.h"

t_bool	valid_quotes(char *cmd)
{
	int	i;
	int	one;
	int	two;
	int	pos;

	i = 0;
	one = 0;
	two = 0;
	while (cmd[i])
	{
		if (cmd[i] == '"' && cmd[i])
		{
			two++;
			pos = i;
			i++;
			while (cmd[i] && cmd[i] != '"')
				i++;
			if (!cmd[i])
				break ;
			else if (cmd[i] == '"')
				two--;
		}
		if (cmd[i] == '\'' && cmd[i])
		{
			one++;
			pos = i;
			i++;
			while (cmd[i] && cmd[i] != '\'')
				i++;
			if (!cmd[i])
				break ;
			else if (cmd[i] == '\'')
				one--;
		}
		i++;
	}
	if (two || one)
	{
		ft_error("minishell: Unmaching opening quotes at col %d: %s\n", 0, pos, cmd + (pos));
		return (false);
	}
	return (true);
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
		return (clean_str(shell, ft_strdup(cmd)));
	return (cmd);
}
