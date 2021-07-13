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
