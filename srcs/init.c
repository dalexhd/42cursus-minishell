#include "../includes/minishell.h"

static void	inout_split(t_redirect *redirect, t_alist *args,
	t_parsed *parsed, t_rstatus *status)
{
	status->status = true;
	status->file = ft_strdup(args->next->content->file);
	args->content = NULL;
	args->next->content = NULL;
	ft_rlstadd_back(&parsed->redirects, ft_rlstnew(redirect));
}

static	void	args_loop(t_alist *args, t_parsed *parsed)
{
	t_redirect	*redirect;

	while (args)
	{
		redirect = (t_redirect *)malloc(sizeof(t_redirect));
		redirect->in = (t_rstatus){.status = false, .file = NULL};
		redirect->out = (t_rstatus){.status = false, .file = NULL};
		redirect->aout = (t_rstatus){.status = false, .file = NULL};
		if (args->content)
		{
			if (args->content->type == R_IN)
				inout_split(redirect, args, parsed, &redirect->in);
			else if (args->content->type == R_OUT)
				inout_split(redirect, args, parsed, &redirect->out);
			else if (args->content->type == R_AOUT)
				inout_split(redirect, args, parsed, &redirect->aout);
			ft_alstadd_back(&parsed->args, ft_alstnew(args->content));
		}
		args = args->next;
	}
}

void	lsh_split_line(t_shell *shell, char *line)
{
	t_list		*tokens;
	t_parsed	*parsed;
	t_alist		*args;

	tokens = ft_safesplitlist(line, '|', "\"'");
	while (tokens)
	{
		parsed = (t_parsed *)malloc(sizeof(t_parsed));
		parsed->args = NULL;
		args = parse_args(shell, fix_cmd(shell, tokens->content));
		if (args)
		{
			parsed->line = ft_strdup(line);
			parsed->flags = (t_flags){
				.has_stdin = !!tokens->prev, .has_stdout = !!tokens->next
			};
			parsed->redirects = NULL;
			args_loop(args, parsed);
			ft_slstadd_back(&shell->parsed, ft_slstnew(parsed));
			tokens = tokens->next;
		}
		else
			break ;
	}
}

static void	fill_data(t_slist *list)
{
	while (list)
	{
		if (list->prev && list->prev->content->flags.has_stdout)
			list->content->flags.has_stdin = true;
		if (list->next && list->next->content->flags.has_stdin)
			list->content->flags.has_stdout = true;
		if (list->next)
			fill_data(list->next);
		break ;
	}
}

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	g_running = true;
	shell->should_wait = true;
	shell->envp = envp;
	shell->parsed = NULL;
	shell->first = true;
	shell->is_cmd = false;
	shell->exit_status = 0;
	return (shell);
}

/* static	void	*terminate(t_shell *shell)
{
	t_slist *list;

	list = shell->parsed;
	while (list)
	{
		waitpid(list->content->pid, NULL, 0);
		list = list->next;
	}
	list = shell->parsed;
} */

void	exec_shell(t_shell *shell, char *cmd)
{
	if (ft_strlen(cmd) > 0)
	{
		shell->parsed = NULL;
		shell->first = false;
		lsh_split_line(shell, cmd);
		shell->pipe_count = ft_slstsize(shell->parsed);
		fill_data(shell->parsed);
		if (shell->exit_status > -1)
			run(shell);
	}
}
