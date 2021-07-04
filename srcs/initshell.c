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
	//if (parsed->line)
	//	free(parsed->line);
	free(parsed);
}

void	del_alst(t_args *args)
{
	if (args && args->cmd)
		ft_strdel(&args->cmd);
	if (args && args->bin_path)
		ft_strdel(&args->bin_path);
	free(args);
}

void	del_rlst(t_redirect *redirect)
{
	if (redirect->aout.status)
		free(redirect->aout.file);
	if (redirect->out.status)
		free(redirect->out.file);
	if (redirect->in.status)
		free(redirect->out.file);
	free(redirect);
}

void	clear_cmd(t_shell *shell)
{
	if (shell->parsed && shell->parsed->content->args)
		ft_alstclear(&shell->parsed->content->args, del_alst);
	if (shell->mierdecilla)
		free(shell->mierdecilla);
	if (shell->parsed && shell->parsed->content->redirects)
		ft_rlstclear(&shell->parsed->content->redirects, del_rlst);
	if (shell->parsed)
		ft_slstclear(&shell->parsed, del_slst);
}

void	exec_shell(t_shell *shell, char *cmd)
{
	if (ft_strlen(cmd) > 0)
	{
		shell->parsed = NULL;
		shell->first = false;
 		lsh_split_line(shell, cmd);
		shell->pipe_count = ft_slstsize(shell->parsed);
		fill_data(shell->parsed);
		if (shell->status > -1)
			run(shell);
		clear_cmd(shell);
	}
}
