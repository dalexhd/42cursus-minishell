#include "../../includes/minishell.h"

static t_bool	wildside(t_shell *shell, char *b)
{
	if (b[0] == 'D' - 64)
	{
		ctld(shell);
	}
	else if (b[0] == 'M' - 64)
	{
		end_tc(shell);
		sandman(shell);
	}
	else
		return (false);
	return (true);
}

static void	neworder(t_shell *shell, char *b)
{
	if (b[0] == 127)
		eraser(shell);
	else if (b[0] == '\\' - 64)
		ctlb(shell);
	else if (b[0] == 'C' - 64)
		ctlc(shell);
	else if (b[0] == 'L' - 64)
		ctll(shell);
	else if (b[0] > 31 && b[0] < 127 && b[1] == 0)
		tear(shell, b[0]);
}

static t_bool	history_repeating(t_shell *shell, char *b)
{
	if (tgetstr("ku", NULL) && !ft_strcmp(b, tgetstr("ku", NULL)))
		history_up(shell);
	else if (tgetstr("kd", NULL) && !ft_strcmp(b, tgetstr("kd", NULL)))
		history_down(shell);
	else
		return (false);
	return (true);
}

void	loureed(t_shell *shell)
{
	char	b[4];
	int		ret;

	ft_bzero(&b, 4);
	while (g_running)
	{
		while (1)
		{
			enable_raw_mode(shell);
			ft_bzero(&b, 4);
			ret = read(STDIN_FILENO, &b, 4);
			if (ret == -1)
				ft_error("Read error #332", true);
			if (wildside(shell, b))
				break ;
			else if (b[0] == 127 || b[0] == '\\' - 64
				|| b[0] == 'C' - 64 || b[0] == 'L' - 64
				|| (b[0] > 31 && b[0] < 127 && b[1] == 0))
				neworder(shell, b);
			history_repeating(shell, b);
		}
		end_tc(shell);
	}
}
