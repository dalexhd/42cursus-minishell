#include "../includes/minishell.h"

static int	quodouble(char c, int quo)
{
	if (c == '"')
		return (!(quo));
	return (quo);
}

static int	quosingle(char c, int quo, int qd)
{
	if (qd && c == '\'')
		return (!(quo));
	return (quo);
}

static int	slowquo(char *cmd, int i)
{
	if (cmd[i - 1] != '<' && cmd[i - 1] != '>' && cmd[i - 1] != ' ')
	{
		cmd = ft_strduplen(cmd, ft_strlen(cmd) + 1);
		ft_insertchar(cmd, ' ', i);
		i++;
	}
	if (cmd[i + 1] != '<' && cmd[i + 1] != '>' && cmd[i + 1] != ' ')
	{
		cmd = ft_strduplen(cmd, ft_strlen(cmd) + 1);
		ft_insertchar(cmd, ' ', i + 1);
		i++;
	}
	return (i);
}

char	*fix_cmd(char *cmd)
{
	size_t	i;
	int		quo[2];

	i = 0;
	quo[0] = 0;
	quo[1] = 0;
	while (i < ft_strlen(cmd))
	{
		quo[0] = quodouble(cmd[i], quo[0]);
		quo[1] = quosingle(cmd[i], quo[1], quo[0]);
		while (quo[0] && quo[1] && cmd[i + 1] != '\'' && cmd[i + 1] != '"')
			i++;
		if ((!quo[0] && !quo[1]) && (cmd[i] == '<' || cmd[i] == '>'))
		{
			i = slowquo(cmd, i);
		}
		i++;
	}
	return (cmd);
}
