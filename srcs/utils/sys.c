#include "../../includes/minishell.h"

void	exec(t_shell *shell, t_parsed *parsed)
{
	char	*arg;
	char	**args;
	char	*pwd;
	t_bool	force_exit;

	force_exit = false;
	if (!parsed->args)
		return ;
	arg = (char *)parsed->args->content->cmd;
	args = ft_safesplit(shell, parsed->args);
	shell->status = 0;
	if (parsed->args->content->is_builtin)
	{
		if (!ft_strcmp(arg, "cd"))
			ft_cd(shell, args);
		else if (!ft_strcmp(arg, "exit"))
			ft_exit(args);
		else if (!ft_strcmp(arg, "export"))
			ft_export(shell, args);
		else if (!ft_strcmp(arg, "unset"))
			ft_unset(shell, args);
		else
		{
			force_exit = true;
			if (!ft_strcmp(arg, "env"))
				ft_env(shell);
			else if (!ft_strcmp(arg, "pwd"))
			{
				pwd = ft_pwd();
				ft_printf("%s\n", pwd);
				ft_strdel(&pwd);
			}
			else if (!ft_strcmp(arg, "echo"))
				ft_echo(args);
		}
	}
	else if (parsed->args->content->bin_path && execve(parsed->args->content->bin_path, args, shell->envp) == -1)
	{
		if (is_directory(parsed->args->content->bin_path))
			ft_error("minishell: %s: is a directory\n", 126, args[0]);
		else if (!has_access(parsed->args->content->bin_path))
			ft_error("minishell: %s: Permission denied\n", 126, args[0]);
		else if (ft_strncmp(parsed->args->content->bin_path, "./", 2) == 0)
			ft_error("minishell: %s: No such file or directory\n", 127, args[0]);
		else
			ft_error("minishell: %s: command not found\n", 127, args[0]);
	}
	ft_split_del(args);
	if (force_exit)
	{
		clear_cmd(shell);
		exit(0);
	}
}
static int	renegado(int **pipes, int j)
{
	close(pipes[j][1]);
	close(pipes[j][0]);
	j++;
	return (j);
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

void	run(t_shell *shell)
{
	pid_t	pid;
	pid_t	*pids;
	t_slist	*list;
	int		**pipes;
	t_bool	*whitelist;
	int		input;
	int		i;
	int		j;
	int		k;
	int		status;

	list = shell->parsed;
	if (!list)
		return ;
	pids = malloc(ft_slstsize(list) * sizeof(int) + 1);
	whitelist = malloc(ft_slstsize(list) * sizeof(t_bool) + 1);
	pipes = (int **) malloc ((ft_slstsize(list) - 1) * sizeof(int *));
	i = 0;
	while (i < ft_slstsize(list) - 1)
	{
		pipes[i] = (int *) malloc (2 * sizeof(int));
		if (pipe(pipes[i]) < 0)
			ft_error("Fallo all crear el pipe %s\n", 1, strerror(errno));
		i++;
	}
	i = 0;
	while (list)
	{
		while (list->content->args)
		{
			if (list->content->args->content)
				break;
			list->content->args = list->content->args->next;
		}
		if (!list->content->args)
			break;
		whitelist[i] = true;
		if (ft_strcmp(list->content->args->content->cmd, "exit") == 0
			|| ft_strcmp(list->content->args->content->cmd, "cd") == 0
			|| ft_strcmp(list->content->args->content->cmd, "export") == 0
			|| ft_strcmp(list->content->args->content->cmd, "unset") == 0)
		{
			exec(shell, list->content);
			whitelist[i] = false;
		}
		else if (ft_slstsize(shell->parsed) == 1)
		{
			pid = fork();
			signal_handler();
			if (pid < 0)
				ft_error("Fallo el fork() %s\n", 1, strerror(errno));
			else if (pid == 0)
			{
				handle_redirect(list);
				exec(shell, list->content);
			}
			else
			{
				waitpid(pid, &status, 0);
				if (WIFSIGNALED(status))
				{
					shell->status = WTERMSIG(status);
					if (shell->status == 2)
						shell->status = 130;
				}
				else if (WIFEXITED(status) != 0 && WEXITSTATUS(status) != 0)
					shell->status = WEXITSTATUS(status);
			}
		}
		else
		{
			pid = fork();
			if (pid < 0)
				ft_error("Fallo el fork() %s\n", 1, strerror(errno));
			else if (pid == 0)
			{
				if (i == 0)
				{
					if (list->content->flags.redirect.in.status)
					{
						input = open(list->content->flags.redirect.in.file,
								O_RDONLY, 0600);
						if (input < 0)
							return (ft_error("minishell: %s: %s\n", true,
									list->content->flags.redirect.in.file,
									strerror(errno)));
						dup2(input, STDIN_FILENO);
					}
					j = 1;
					while (j < ft_slstsize(shell->parsed) - 1)
						j = renegado(pipes, j);
					close(pipes[0][0]);
					dup2(pipes[0][1], 1);
				}
				if (i > 0 && i < ft_slstsize(shell->parsed) - 1)
				{
					if (i == 1 && ft_slstsize(shell->parsed) != 3)
					{
						j = i + 1;
						while (j < ft_slstsize(shell->parsed) - 1)
							j = renegado(pipes, j);
					}
					if (i == ft_slstsize(shell->parsed) - 2
						&& ft_slstsize(shell->parsed) != 3)
					{
						j = 0;
						while (j < i - 1)
							j = renegado(pipes, j);
					}
					if (i != 1 && i != ft_slstsize(shell->parsed) - 2
						&& ft_slstsize(shell->parsed) != 3)
					{
						j = 0;
						while (j < i - 1)
							j = renegado(pipes, j);
						j = i + 1;
						while (j < ft_slstsize(shell->parsed) - 1)
							j = renegado(pipes, j);
					}
					close(pipes[i - 1][1]);
					dup2(pipes[i - 1][0], 0);
					close(pipes[i][0]);
					dup2(pipes[i][1], 1);
				}
				if (i == ft_slstsize(shell->parsed) - 1)
				{
					handle_redirect(list);
					j = 0;
					while (j < ft_slstsize(shell->parsed) - 2)
						j = renegado(pipes, j);
					close(pipes[i - 1][1]);
					dup2(pipes[i - 1][0], 0);
				}
				exec(shell, list->content);
			}
			else
				pids[i] = pid;
		}
		list = list->next;
		i++;
	}
	k = 0;
	while (k < ft_slstsize(shell->parsed) - 1)
		k = renegado(pipes, k);
	k = 0;
	while (k < ft_slstsize(shell->parsed))
	{
		if (whitelist[k])
		{
			waitpid(pids[k], &status, 0);
			if (WIFSIGNALED(status))
			{
				shell->status = WTERMSIG(status);
				if (shell->status == 2)
					shell->status = 130;
			}
			else if (WIFEXITED(status) != 0 && WEXITSTATUS(status) != 0)
				shell->status = WEXITSTATUS(status);
			else if (WEXITSTATUS(status) == 0)
				shell->status = 0;
		}
		k++;
	}
	i = 0;
	while (i < ft_slstsize(shell->parsed) - 1)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
	free(whitelist);
	free(pids);
}
