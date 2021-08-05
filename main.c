#include "includes/minishell.h"

void	del_envlst(t_env *env)
{
	if (env->key)
		ft_strdel(&env->key);
	if (env->val)
		ft_strdel(&env->val);
	free(env);
}

static int	shell_clear(t_shell *shell)
{
	int	status;

	status = shell->status;
	if (shell->tmp_dir)
		free(shell->tmp_dir);
	if (shell->envp)
		ft_split_del(shell->envp);
	if (shell->envp_2)
		ft_envlstclear(&shell->envp_2, del_envlst);
	free(shell->envp_2);
	ft_hlstclear(&shell->term.history, free);
	free(shell);
	if (status != 0)
		exit(status);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell		*shell;

	shell = init_shell(envp);
	signal_handler();
	if (argc != 3)
	{
		init_tc(shell);
		ft_printshell(shell);
		loureed(shell);
		end_tc(shell);
	}
	else
	{
		if (argv[2][0] == ';')
			sh_error(shell, ERR_UT, 1, ";");
		else
			parse_commands(shell, argv[2]);
	}
	return (shell_clear(shell));
}
