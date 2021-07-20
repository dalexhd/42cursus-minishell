#include "../../../includes/minishell.h"

static	void	handle_double(char *cmd, char out, int *i)
{
	cmd[*i] = DEL;
	while (cmd[*i])
	{
		if (cmd[*i] == out && cmd[*i - 1] != '\\')
			break ;
		(*i)++;
	}
	if (cmd[*i] == out)
		cmd[*i] = DEL;
}

static	void	handle_single(char *cmd, t_bool *flag, char out, int *i)
{
	char		*tmp;

	cmd[*i] = DEL;
	while (cmd[*i])
	{
		*flag = true;
		if (*i > 0 && cmd[*i - 1] == '\\')
			*flag = false;
		if (cmd[*i] == out)
			break ;
		else if (!flag && cmd[*i] != out && cmd[*i] != '"')
		{
			tmp = ft_strjoin(ft_strduplen(cmd, *i - 1), "\\\\");
			cmd = ft_strdup(ft_strjoin_free(tmp, cmd + *i));
			*i += 2;
		}
		(*i)++;
	}
	if (cmd[*i] == out)
		cmd[*i] = DEL;
}

static	char	*ret_cmd(char *cmd)
{
	int		j;
	int		i;
	char	*tmp;

	i = 0;
	j = 0;
	tmp = ft_strdup(cmd);
	while (i < (int)ft_strlen(cmd))
	{
		if (cmd[i] != DEL)
		{
			tmp[j] = cmd[i];
			j++;
		}
		i++;
	}
	tmp[j] = '\0';
	ft_strdel(&cmd);
	return (tmp);
}

char	*remove_cmd_quotes(char *cmd)
{
	int		i;
	int		fl;

	i = 0;
	fl = 0;
	while (i < (int)ft_strlen(cmd))
	{
		if (ft_strchr("\"'", cmd[i])
			&& (!fl || fl & 1 << (ft_strchr("\"'", cmd[i]) - "\"'")))
			fl ^= 1 << (ft_strchr("\"'", cmd[i]) - "\"'");
		if (i > 0 && cmd[i - 1] == '\\')
			fl = 0;
		if (cmd[i] != '\\' && fl && (cmd[i] == '\'' || cmd[i] == '"'))
		{
			if (cmd[i] == '"')
				handle_double(cmd, cmd[i], &i);
			else if (cmd[i] == '\'')
				handle_single(cmd, &fl, cmd[i], &i);
			fl = 0;
		}
		i++;
	}
	return (ret_cmd(cmd));
}
