#include "../../includes/minishell.h"

static	void	dup_close(int old, int new)
{
	if (dup2(old, new) < 0)
		ft_error("Could not dup: %s\n", false, strerror(errno));
	if (close(old) < 0)
		ft_error("Could not close: %s\n", false, strerror(errno));
}

static	void	redirect_dup_close(t_alist *args, int type)
{
	t_redirect	*redirect;

	redirect = args->content->redirect;
	if (redirect->out.status || redirect->in.status || redirect->aout.status)
	{
		if (type == R_IN && redirect->in.status && redirect->in.fd != 0)
			dup_close(redirect->in.fd, 0);
		else if (type == R_OUT && redirect->out.status && redirect->out.fd != 1)
			dup_close(redirect->out.fd, 1);
	}
	if (args->next)
		redirect_dup_close(args->next, type);
}

void	ft_exec_child(t_shell *shell, t_slist *parsed, int *fd, int prepipe)
{
	t_redirect	*redirect;

	redirect = parsed->content->args->content->redirect;
	if (prepipe)
		dup_close(prepipe, 0);
	redirect_dup_close(parsed->content->args, R_IN);
	if (parsed->next)
		dup_close(fd[1], 1);
	redirect_dup_close(parsed->content->args, R_OUT);
	if (parsed->next)
		close(fd[0]);
	if (!(redirect->out.status || redirect->in.status || redirect->aout.status))
	{
		if (parsed->content->args->content->is_builtin)
			exit(ft_exec_builtin(shell, parsed));
		ft_exec_bin(shell, parsed);
	}
}

void	ft_exec_cmd(t_shell *shell, t_slist *parsed, int prepipe)

{
	pid_t	pid;
	int		fd[2];
	int		status;

	status = 0;
	if (parsed->next && pipe(fd) < 0)
		ft_error("bad pipe openning", 1);
	pid = fork();
	if (pid < 0)
		ft_error("Bad fork!", 1);
	if (pid == 0)
		ft_exec_child(shell, parsed, fd, prepipe);
	if (prepipe && close(prepipe) < 0)
		ft_error("Bad close!", 1);
	if (parsed->next && close(fd[1]) < 0)
		ft_error("Close error: %s\n", 1, strerror(errno));
	if (parsed->next)
		ft_exec_cmd(shell, parsed->next, fd[0]);
	if (waitpid(pid, &status, 0) < 0)
		ft_error("Waitpid error: %s\n", 1, strerror(errno));
	if (!parsed->next && WIFSIGNALED(status))
		shell->status = WTERMSIG(status) + 12;
	else if (!parsed->next)
		shell->status = WEXITSTATUS(status);
}

/*
** - If we only have 1 command, and it's an minishell builtin
**   we need to execute it without forks.
** - If we get more thatn 1 arguments, then we call `ft_exec_cmd`
**   recursive function.
*/
void	run(t_shell *shell)
{
	if (ft_slstsize(shell->parsed) == 1
		&& shell->parsed->content->args->content->is_builtin)
	{
		redirect_dup_close(shell->parsed->content->args, R_OUT);
		ft_exec_builtin(shell, shell->parsed);
	}
	else if (shell->parsed)
		ft_exec_cmd(shell, shell->parsed, 0);
}
