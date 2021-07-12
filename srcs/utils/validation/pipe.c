#include "../../../includes/minishell.h"

t_bool	valid_commas(t_shell *shell, char *cmd)
{
	if (cmd[0] == ';')
	{
		ft_error("minishell: syntax error near unexpected token `;'\n", 0);
		shell->status = 1;
		shell->parsed->content->valid = false;
		return (false);
	}
	return (true);
}
