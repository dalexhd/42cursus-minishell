#include "../../../includes/minishell.h"

static t_bool	check_error(t_shell *shell, char *cmd, int pos, char one)
{
	if (one == '<' || one == '>')
	{
		ft_error("minishell: syntax error near redirect: %s\n",
			0, cmd + (pos));
		shell->status = 2;
		return (false);
	}
	return (true);
}

t_bool	valid_redirects(t_shell *shell, char *cmd, int *i)
{
	char	two;
	int		pos;
	int		red;

	two = 0;
	if (cmd[*i] == '<' || cmd[*i] == '>')
	{
		two = cmd[*i];
		if (cmd[*i] == '>' && cmd[*i + 1] == '>' && cmd[*i + 1])
			(*i)++;
		pos = (*i)++;
		while (cmd[*i] == ' ')
			(*i)++;
		if (cmd[*i] != '>' && cmd[*i] != '<')
			red = 0;
		else
			return (check_error(shell, cmd, pos, two));
	}
	return (true);
}
