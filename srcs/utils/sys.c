#include "../../includes/minishell.h"

/* static void	handle_redirect(t_slist	*list)
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
} */

static	void	dup_close(int old, int new)
{
	if (dup2(old, new) < 0)
		ft_error("Could not dup!", STDOUT_FILENO);
	if (close(old) < 0)
		ft_error("Could not close!", STDOUT_FILENO);
}

/**
 * Here we exec the builtin
 *
**/
void	ft_exec_builtin(t_shell *shell, t_slist *parsed)
{
	(void)shell;
	(void)parsed;
	return ;
}

/**
 * Here we exec the system builtin
 *
**/
void	ft_exec_bin(t_shell *shell, t_slist *parsed)
{
	t_alist	*args;
	char	**args_split;

	args = parsed->content->args;
	args_split = ft_safesplit(shell, args);
	if (!args_split[0])
		exit(0);
	if (args->content->bin_path
		&& execve(args->content->bin_path, args_split, shell->envp) == -1)
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

void	ft_exec_child(t_shell *shell, t_slist *parsed, int *fd, int prepipe)
{
	if (prepipe)
		dup_close(prepipe, 0);
	if (parsed->content->flags.has_stdin != 0)
		dup_close(STDIN_FILENO, 0);
	if (parsed->next)
		dup_close(fd[1], 1);
	if (parsed->content->flags.has_stdout != 1)
		dup_close(STDOUT_FILENO, 0);
	if (parsed->next)
		close(fd[0]);
	if (parsed->content->args->content->is_builtin)
		ft_exec_builtin(shell, parsed);
	ft_exec_bin(shell, parsed);
}

void	ft_exec_cmd(t_shell *shell, t_slist *parsed, int prepipe)

{
	pid_t	pid;
	int		fd[2];
	int		status;

	status = 0;
	if (parsed->next && pipe(fd) < 0)
	{
		ft_error("bad pipe openning", false);
		return ;
	}
	pid = fork();
	if (pid < 0)
	{
		ft_error("Bad fork!", false);
		return ;
	}
	if (pid == 0)
		ft_exec_child(shell, parsed, fd, prepipe);
	if (prepipe && close(prepipe) < 0)
	{
		ft_error("Bad close!", false);
		return ;
	}
	if (parsed->next && close(fd[1]) < 0)
	{
		ft_error("Bad pipe!", false);
		return ;
	}
	if (parsed->next)
		ft_exec_cmd(shell, parsed->next, fd[0]);
	if (waitpid(pid, &status, 0) < 0)
	{
		ft_error("Bad pipe!", false);
		return ;
	}
	if (parsed->next)
		shell->status = WEXITSTATUS(status);
	if (!parsed->next && WIFSIGNALED(status))
		shell->status = WTERMSIG(status) + 128;
}

/*
**	- If we only have 1 command, and it's an minishell builtin
**	we need to execute it without forks.
**	- If we get more thatn 1 arguments, then we call `ft_exec_cmd`
**	recursive function.
*/
void	run(t_shell *shell)
{
	if (shell->parsed)
	{
		ft_exec_cmd(shell, shell->parsed, 0);
	}
}
