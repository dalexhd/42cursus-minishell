#include "../includes/minishell.h"

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	bzero(shell, sizeof(t_shell));
	g_running = true;
	shell->should_wait = true;
	shell->envp = ft_split_dup(envp);
	shell->parsed = NULL;
	shell->first = true;
	shell->is_cmd = false;
	shell->status = 0;
	shell->envp_2 = ft_setup_env(shell, envp);
	shell->tmp_dir = ft_strjoin(shell->home_dir, "/.minishell_history");
	shell_lvl(shell);
	return (shell);
}

void	del_alst(t_args *args)
{
	if (args && args->cmd)
		ft_strdel(&args->cmd);
	if (args && args->bin_path)
		ft_strdel(&args->bin_path);
	if (args)
		free(args->redirect);
	if (args && args->file)
		ft_strdel(&args->file);
	free(args);
}

void	del_slst(t_parsed *parsed)
{
	if (parsed->line)
		free(parsed->line);
	if (parsed->redirects)
		ft_rlstclear(&parsed->redirects, free);
	if (parsed->args)
		ft_alstclear(&parsed->args, del_alst);
	free(parsed);
}

void	clear_cmd(t_shell *shell)
{
	if (shell->mierdecilla)
		free(shell->mierdecilla);
	if (shell->parsed)
		ft_slstclear(&shell->parsed, del_slst);
}

void	exec_shell(t_shell *shell, char *cmd)
{
	if (g_running && ft_strlen(cmd) > 0)
	{
		shell->is_cmd = true;
		shell->parsed = NULL;
		shell->first = false;
		shell->mierdecilla = NULL;
		lsh_split_line(shell, cmd);
		shell->pipe_count = ft_slstsize(shell->parsed);
		if (shell->pipe_count > 0 && !shell->parsed->content->args)
			shell->parsed->content->valid = false;
		if (shell->status > -1 && shell->parsed
			&& shell->parsed->content->valid)
			run(shell);
		clear_cmd(shell);
	}
}
