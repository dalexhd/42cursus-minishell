#include "../../../includes/minishell.h"

static	t_bool	check_tilde(char *cmd)
{
	size_t	len;

	len = ft_strlen(cmd);
	if (cmd[0] != '~')
		return (false);
	else if (len == 1 && cmd[0] == '~')
		return (true);
	else if (len > 0 && cmd[0] == '~' && cmd[1] == '/')
		return (true);
	else
		return (false);
}

void	parse_tilde(t_shell *shell, char *cmd, int *i, char **line)
{
	char	*tmp;

	if (check_tilde(cmd))
	{
		tmp = ft_strnew(1);
		if (shell->home_dir)
			ft_strcat(tmp, shell->home_dir);
		while (cmd[*i] && !ft_isspace(cmd[*i]))
		{
			if (cmd[*i] != '~')
				ft_strncat(tmp, &cmd[*i], 1);
			(*i)++;
		}
		(*i)--;
		*line = ft_strjoin_free(*line, tmp);
	}
	else
		ft_strcat(*line, "~");
}
