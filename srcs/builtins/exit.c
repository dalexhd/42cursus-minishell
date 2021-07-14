#include "../../includes/minishell.h"

void	ft_exit(t_shell *shell, char **args)
{
	size_t	i;
	char	*arg;
	t_llong	number;

	i = 1;
	arg = ft_strtrim(args[1], " ");
	number = ft_atoll(arg);
	if ((((arg[0] == '-' && (!arg[1] || !ft_strevery(&arg[1], ft_isdigit)))
				|| (arg[0] != '-' && !ft_strevery(arg, ft_isdigit)))
			&& ((arg[0] == '+' && (!arg[1]
						|| !ft_strevery(&arg[1], ft_isdigit)))
				|| (arg[0] != '+' && !ft_strevery(arg, ft_isdigit))))
		|| ((ft_strcmp(arg, "-1") && number == -1) || ft_strlen(arg) > 19))
		sh_error(shell, "minishell: exit: %s: numeric argument required\n", 2,
			args[i]);
	else if (args[i + 1])
		sh_error(shell, "minishell: exit: too many arguments\n", 1);
	else
		shell->status = number;
	g_running = false;
}
