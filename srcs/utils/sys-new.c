#include "../../includes/minishell.h"

/*
** Here we exec builtins. If the command is not a builtin, then
** we willl return false.
*/

static t_bool	check_builtin(t_shell *shell, t_slist *list)
{
	t_args		*content;
	char		**args;

	content = list->content->args->content;
	if (!content->is_builtin)
		return (false);
	args = ft_safesplit(shell, list->content->args);
	if (!ft_strcmp(content->cmd, "cd"))
		ft_cd(shell, args);
	else if (!ft_strcmp(content->cmd, "exit"))
		ft_exit(args);
	else if (!ft_strcmp(content->cmd, "export"))
		ft_export(shell, args);
	else if (!ft_strcmp(content->cmd, "unset"))
		ft_unset(shell, args);
	else if (!ft_strcmp(content->cmd, "env"))
		ft_env(shell);
	else if (!ft_strcmp(content->cmd, "pwd"))
		ft_printf("%s\n", ft_pwd());
	else if (!ft_strcmp(content->cmd, "echo"))
		ft_echo(args);
	return (true);
}

static void	handle_redirect(t_slist	*list)
{
	int		input;
	t_rlist	*redirect;

	redirect = list->content->redirects;
	while (redirect)
	{
		if (redirect->content->in.status)
		{
			input = open(redirect->content->in.file, O_RDONLY, 0600);
			if (input < 0)
				return (ft_error("minishell: %s: %s\n", true,
						redirect->content->in.file, strerror(errno)));
			dup2(input, STDIN_FILENO);
		}
		else if (redirect->content->out.status)
		{
			input = open(redirect->content->out.file,
					O_TRUNC | O_WRONLY | O_CREAT, 0600);
			if (input < 0)
				return (ft_error("minishell: %s: %s\n", true,
						redirect->content->out.file, strerror(errno)));
			dup2(input, STDOUT_FILENO);
		}
		else if (redirect->content->aout.status)
		{
			input = open(redirect->content->aout.file,
					O_WRONLY | O_CREAT | O_APPEND, 0600);
			if (input < 0)
				return (ft_error("minishell: %s: %s\n", true,
						redirect->content->aout.file, strerror(errno)));
			dup2(input, STDOUT_FILENO);
		}
		redirect = redirect->next;
	}
}

static void	exec_bin(t_shell *shell, t_slist *list)
{
	pid_t	pid;
	t_alist	*args;
	char	**parsed_args;

	args = list->content->args;
	parsed_args = ft_safesplit(shell, args);
	pid = fork();
	ft_plstadd_back(&shell->pids, ft_plstnew(&pid));
	if (pid == 0)
	{
		handle_redirect(list);
		if (execve(args->content->bin_path, parsed_args, shell->envp) == -1)
		{
			if (is_directory(args->content->bin_path))
				ft_error("minishell: %s: is a directory\n", 126, args[0]);
			else if (!has_access(args->content->bin_path))
				ft_error("minishell: %s: Permission denied\n", 126, args[0]);
			else if (ft_strncmp(args->content->bin_path, "./", 2) == 0)
				ft_error("minishell: %s: No such file or directory\n", 127, args[0]);
			else
				ft_error("minishell: %s: command not found\n", 127, args[0]);
		}
	}
}

static void	exec_command(t_shell *shell, t_slist *list)
{
	if (check_builtin(shell, list))
		return ;
	exec_bin(shell, list);
}

static void	handle_zombies(t_shell *shell)
{
	int		status;

	if (shell->pids)
	{
		waitpid(*shell->pids->content, &status, 0);
		if (WIFEXITED(status) != 0 && WEXITSTATUS(status) != 0)
			shell->status = WEXITSTATUS(status);
		else if (WEXITSTATUS(status) == 0)
			shell->status = 0;
		shell->pids = shell->pids->next;
	}
}

void	run(t_shell *shell)
{
	t_slist	*list;

	list = shell->parsed;
	if (!list)
		return ;
	shell->pids = (t_pids *)malloc(sizeof(t_pids));
	shell->pids = NULL;
	while (list)
	{
		while (list->content->args)
		{
			if (list->content->args->content)
				break ;
			list->content->args = list->content->args->next;
		}
		exec_command(shell, list);
		list = list->next;
	}
	handle_zombies(shell);
	free(shell->pids);
}
