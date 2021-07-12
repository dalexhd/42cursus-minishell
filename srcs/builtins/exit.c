#include "../../includes/minishell.h"

void	ft_exit(t_shell *shell, char **args)
{
	size_t	i;
	char	*arg;
	t_llong	number;

	i = 1;
	if (!args[1])
		g_running = false;
	arg = ft_strtrim(args[1], " ");
	number = ft_atoll(arg);
	if ((((arg[0] == '-' && (!arg[1] || !ft_strevery(&arg[1], ft_isdigit)))
				|| (arg[0] != '-' && !ft_strevery(arg, ft_isdigit)))
			&& ((arg[0] == '+' && (!arg[1]
						|| !ft_strevery(&arg[1], ft_isdigit)))
				|| (arg[0] != '+' && !ft_strevery(arg, ft_isdigit))))
		|| ((ft_strcmp(arg, "-1") && number == -1) || ft_strlen(arg) > 19))
	{
		ft_error("minishell: exit: %s: numeric argument required\n",
			0, args[i]);
		g_running = false;
		shell->status = 2;
	}
	else if (args[i + 1])
	{
		ft_error("minishell: exit: too many arguments\n", 0);
		g_running = false;
		shell->status = 1;
	}
	else
	{
		g_running = false;
		shell->status = number;
	}
}
