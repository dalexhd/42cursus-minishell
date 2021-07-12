#include "../../../includes/minishell.h"

static void	quotes(char *cmd, int *i, char c)
{
	while (cmd[*i] && cmd[*i] != c)
		(*i)++;
	if (!cmd[*i])
		*i = -42;
}

static t_bool	check_error(t_shell *shell, char *cmd, int *pos, char one)
{
	if (one == '\'' || one == '"')
	{
		ft_error("minishell: Unmaching opening quotes at col %d: %s\n",
			0, *pos, cmd + *pos);
		shell->status = 2;
		shell->parsed->content->valid = false;
		return (false);
	}
	return (true);
}

t_bool	valid_quotes(t_shell *shell, char *cmd, int *i)
{
	char	one;
	int		pos;

	if ((cmd[*i] == '\'' || cmd[*i] == '"') && cmd[*i])
	{
		if (*i > 0 && cmd[*i - 1] == '\\')
		{
			(*i)++;
			return (true);
		}
		one = cmd[*i];
		pos = (*i)++;
		quotes(cmd, i, one);
		if (*i == -42)
			return (check_error(shell, cmd, &pos, one));
		else if (cmd[*i] == one)
			one = 0;
	}
	return (true);
}
