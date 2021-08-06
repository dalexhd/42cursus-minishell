#include "../../../includes/minishell.h"

t_bool	valid_commas(t_shell *shell, char *cmd)
{
	if (cmd[0] == ';')
	{
		sh_error(shell, ERR_UT, 1, ";");
		shell->parsed->content->valid = false;
		return (false);
	}
	return (true);
}

static void	quotes(char *cmd, int *i, int *e, char c)
{
	int		num;
	int		num2;
	t_bool	ret;

	num = 0;
	*e = *i;
	while (cmd[*i] && cmd[*i + 1] != c)
	{
		(*i)++;
		num++;
	}
	ret = false;
	num2 = 0;
	while (num2 < num)
	{
		if (cmd[(*i - num) + num2] != ' ' && cmd[(*i - num) + num2] != '\0'
			&& cmd[(*i - num) + num2] != c)
			ret = true;
		num2++;
	}
	if (!ret)
		*i = -42;
}

static t_bool	check_error(t_shell *shell, char *cmd, int *pos, char one)
{
	if (one == '|')
	{
		sh_error(shell, ERR_UT, 1, cmd + *pos);
		return (false);
	}
	return (true);
}

t_bool	valid_pipes(t_shell *shell, char *cmd)
{
	char	one;
	int		i;
	int		e;

	i = 0;
	if (cmd[0] == '|')
	{
		sh_error(shell, ERR_UT, 1, "|");
		return (false);
	}
	while (i < (int)ft_strlen(cmd))
	{
		if (cmd[i] == '|')
		{
			if (i > 0 && cmd[i - 1] == '\\')
				return (true);
			one = cmd[i];
			i++;
			quotes(cmd, &i, &e, one);
			if (i == -42)
				return (check_error(shell, cmd, &e, one));
		}
		i++;
	}
	return (true);
}
