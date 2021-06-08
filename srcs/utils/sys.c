#include "../../includes/minishell.h"

void	exec(t_shell *shell, t_parsed *parsed)
{
	char	*arg;
	char	**args;

	arg = (char *)parsed->args->content->cmd;
	args = ft_safesplit(parsed->args);
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
			if (!ft_strcmp(arg, "env"))
				ft_env(shell);
			else if (!ft_strcmp(arg, "pwd"))
				ft_printf("%s\n", ft_pwd());
			else if (!ft_strcmp(arg, "echo"))
				ft_echo(args);
			exit(0);
		}
	}
	else if (execve(parsed->args->content->bin_path, args, shell->envp) == -1)
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
				return (ft_error("minishell: %s: %s\n", true, redirect->content->in.file, strerror(errno)));
			dup2(input, STDIN_FILENO);
		}
		else if (redirect->content->out.status)
		{
			input = open(redirect->content->out.file, O_TRUNC | O_WRONLY | O_CREAT, 0600);
			if (input < 0)
				return (ft_error("minishell: %s: %s\n", true, redirect->content->out.file, strerror(errno)));
			dup2(input, STDOUT_FILENO);
		}
		else if (redirect->content->aout.status)
		{
			input = open(redirect->content->aout.file, O_WRONLY | O_CREAT | O_APPEND, 0600);
			if (input < 0)
				return (ft_error("minishell: %s: %s\n", true, redirect->content->aout.file, strerror(errno)));
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
	int		input;
	int		i;
	int		s;
	int		j;
	int		k;
	int		status;

	list = shell->parsed;
	if (!list)
		return ;
	pids = malloc(ft_slstsize(shell->parsed) * sizeof(int) + 1);
	pipes = (int **) malloc ((ft_slstsize(shell->parsed) - 1) * sizeof(int *));
	i = 0;
	while (i < ft_slstsize(shell->parsed) - 1)
	{
		pipes[i] = (int *) malloc (2 * sizeof(int));
		if (pipe(pipes[i]) < 0)
			ft_error("Fallo all crear el pipe %s\n", 1, strerror(errno));
		i++;
	}
	i = 0;
	s = 0;
	while (list)
	{
		while (list->content->args)
		{
			if (list->content->args->content)
				break ;
			list->content->args = list->content->args->next;
		}
		input = 0;
		if (/*list->content->args->content && */ft_strcmp(list->content->args->content->cmd, "exit") == 0)
		{
			exec(shell, list->content);
		}
		else if (/*list->content->args->content && */ft_strcmp(list->content->args->content->cmd, "cd") == 0)
		{
			exec(shell, list->content);
		}
		else if (ft_slstsize(shell->parsed) == 1) //If there is only a single command
		{
			pid = fork();
			if (pid < 0)
				ft_error("Fallo el fork() %s\n", 1, strerror(errno));
			else if (pid == 0) // Hijo
			{
				handle_redirect(list);
				exec(shell, list->content);
			}
			else
			{
				waitpid(pid, &status, 0); // Wait for the children
				if (WIFEXITED(status) != 0 && WEXITSTATUS(status) != 0) {
					shell->exit_status = WEXITSTATUS(status);
				}
			}
		}
		else
		{
			pid = fork();
			if (pid < 0)
				ft_error("Fallo el fork() %s\n", 1, strerror(errno));
			else if (pid == 0) // Children
			{
				if (i == 0)
				{
					if (list->content->flags.redirect.in.status)
					{
						input = open(list->content->flags.redirect.in.file,
								O_RDONLY, 0600);
						if (input < 0)
							return (ft_error("minishell: %s: %s\n", true, list->content->flags.redirect.in.file, strerror(errno)));
						dup2(input, STDIN_FILENO);
					}
					j = 1;
					while (j < ft_slstsize(shell->parsed) - 1)
					{
						close(pipes[j][1]);
						close(pipes[j][0]);
						j++;
					}
					close(pipes[0][0]);
					dup2(pipes[0][1], 1);
				}
				if (i > 0 && i < ft_slstsize(shell->parsed) - 1)
				{
					if (i == 1 && ft_slstsize(shell->parsed) != 3)
					{
						j = i + 1;
						while (j < ft_slstsize(shell->parsed) - 1)
						{
							close(pipes[j][1]);
							close(pipes[j][0]);
							j++;
						}
					}
					if (i == ft_slstsize(shell->parsed) - 2
						&& ft_slstsize(shell->parsed) != 3)
					{
						j = 0;
						while (j < i - 1)
						{
							close(pipes[j][1]);
							close(pipes[j][0]);
							j++;
						}
					}
					if (i != 1 && i != ft_slstsize(shell->parsed) - 2
						&& ft_slstsize(shell->parsed) != 3)
					{
						j = 0;
						while (j < i - 1)
						{
							close(pipes[j][1]);
							close(pipes[j][0]);
							j++;
						}
						j = i + 1;
						while (j < ft_slstsize(shell->parsed) - 1)
						{
							close(pipes[j][1]);
							close(pipes[j][0]);
							j++;
						}
					}
					close(pipes[i - 1][1]);
					dup2(pipes[i - 1][0], 0);
					close(pipes[i][0]);
					dup2(pipes[i][1], 1);
				}
				if (i == ft_slstsize(shell->parsed) - 1)
				{
					if (list->content->flags.redirect.out.status)
					{
						input = open(list->content->flags.redirect.out.file, O_TRUNC | O_WRONLY | O_CREAT, 0600);
						if (input < 0)
							return (ft_error("minishell: %s: %s\n", true, list->content->flags.redirect.out.file, strerror(errno)));
						dup2(input, STDOUT_FILENO);
					}
					else if (list->content->flags.redirect.aout.status)
					{
						input = open(list->content->flags.redirect.aout.file, O_WRONLY | O_APPEND, 0600);
						if (input < 0)
							return (ft_error("minishell: %s: %s\n", true, list->content->flags.redirect.aout.file, strerror(errno)));
						dup2(input, STDOUT_FILENO);
					}
					j = 0;
					while (j < ft_slstsize(shell->parsed) - 2)
					{
						close(pipes[j][1]);
						close(pipes[j][0]);
						j++;
					}
					close(pipes[i - 1][1]);
					dup2(pipes[i - 1][0], 0);
				}
				exec(shell, list->content);
			}
			else // Parent
			{
				pids[i] = pid; // We save the PID of the children
			}
		}
		list = list->next;
		i++;
	}
	k = 0;
	while (k < ft_slstsize(shell->parsed) - 1)
	{
		close(pipes[k][1]);
		close(pipes[k][0]);
		k++;
	}
	k = 0;
	while (k < ft_slstsize(shell->parsed))
	{
		waitpid(pids[k], &status, 0); // Wait for the children
		if (WIFEXITED(status) != 0 && WEXITSTATUS(status) != 0) {
			shell->exit_status = WEXITSTATUS(status);
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
	free(pids);
}
