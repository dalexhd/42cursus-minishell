#include "../../../includes/minishell.h"

void	parse_tilde(t_shell *shell, char *cmd, int *i, char *line)
{
	char	*env;
	char	*tmp;

	tmp = ft_strnew(1);
	if (shell->home_dir)
		ft_strcat(tmp, env);
	while (cmd[*i] && !ft_isspace(cmd[*i]))
	{
		if (cmd[*i] != '~')
			ft_strncat(tmp, &cmd[*i], 1);
		(*i)++;
	}
	(*i)--;
	ft_strcat(line, tmp);
}
