#include "../includes/minishell.h"

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	bzero(shell, sizeof(t_shell));
	g_running = true;
	shell->should_wait = true;
	shell->envp = envp;
	shell->parsed = NULL;
	shell->first = true;
	shell->is_cmd = false;
	shell->status = 0;
	shell->home_dir = ft_getenv(shell, "HOME");
	shell->tmp_dir = ft_strjoin(shell->home_dir, "/.minishell_history");
	return (shell);
}

void	del_slst(t_parsed *parsed)
{
	if (parsed->line)
		free(parsed->line);
	free(parsed);
}

void	del_alst(t_args *args)
{
	if (args && args->cmd)
		ft_strdel(&args->cmd);
	if (args)
	{
		if (args->redirect->in.status)
			ft_strdel(&args->redirect->in.file);
		if (args->redirect->out.status)
			ft_strdel(&args->redirect->out.file);
		if (args->redirect->aout.status)
			ft_strdel(&args->redirect->aout.file);
		free(args->redirect);
	}
	if (args && args->bin_path)
		ft_strdel(&args->bin_path);
	if (args && args->file)
		ft_strdel(&args->file);
	free(args);
}

void	clear_cmd(t_shell *shell)
{
	if (shell->parsed && shell->parsed->content->args)
		ft_alstclear(&shell->parsed->content->args, del_alst);
	if (shell->mierdecilla)
		free(shell->mierdecilla);
	if (shell->parsed)
		ft_slstclear(&shell->parsed, del_slst);
}

void	exec_shell(t_shell *shell, char *cmd)
{
	if (ft_strlen(cmd) > 0)
	{
		shell->is_cmd = true;
		shell->parsed = NULL;
		shell->first = false;
		shell->mierdecilla = NULL;
		lsh_split_line(shell, cmd);
		shell->pipe_count = ft_slstsize(shell->parsed);
		fill_data(shell->parsed);
		if (shell->status > -1)
			run(shell);
		clear_cmd(shell);
	}
}
