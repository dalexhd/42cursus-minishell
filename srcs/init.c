#include "../includes/minishell.h"

static int	split(t_redirect *redirect, t_alist *args,
	t_parsed *parsed, t_rstatus *status)
{
	status->status = true;
	if (!args->next)
	{
		ft_error("minishell: syntax error near unexpected token `newline'\n",
			false);
		return (2);
	}
	status->file = ft_strdup(args->next->content->file);
	if (args && args->content->cmd)
		ft_strdel(&args->content->cmd);
	if (args && args->content->bin_path)
		ft_strdel(&args->content->bin_path);
	args->content = NULL;
	if (args->next && args->next->content->cmd)
		ft_strdel(&args->next->content->cmd);
	if (args->next && args->next->content->bin_path)
		ft_strdel(&args->next->content->bin_path);
	args->next->content = NULL;
	ft_rlstadd_back(&parsed->redirects, ft_rlstnew(redirect));
	return (0);
}

static	void	args_loop(t_shell *shell, t_alist *args, t_parsed *parsed)
{
	t_redirect	*redirect;

	while (args)
	{
		if (args->content)
		{
			if (args->content->type == R_IN
				|| args->content->type == R_OUT
				|| args->content->type == R_AOUT)
			{
				redirect = (t_redirect *)malloc(sizeof(t_redirect));
				redirect->in = (t_rstatus){.status = false, .file = NULL};
				redirect->out = (t_rstatus){.status = false, .file = NULL};
				redirect->aout = (t_rstatus){.status = false, .file = NULL};
				if (args->content->type == R_IN)
					shell->status = split(redirect, args, parsed, &redirect->in);
				else if (args->content->type == R_OUT)
					shell->status = split(redirect, args, parsed, &redirect->out);
				else if (args->content->type == R_AOUT)
					shell->status = split(redirect, args, parsed, &redirect->aout);
			}
			ft_alstadd_back(&parsed->args, ft_alstnew(args->content));
		}
		else
		{
			ft_printf("None char!!!!\n");
		}
		args = args->next;
	}
}

void	lsh_split_line(t_shell *shell, char *line)
{
	t_list		*tokens;
	t_list		*tokens_tmp;
	t_parsed	*parsed;
	t_alist		*args;

	tokens = ft_safesplitlist(line, '|', "\"'");
	tokens_tmp = tokens;
	while (tokens)
	{
		parsed = (t_parsed *)malloc(sizeof(t_parsed));
		parsed->args = NULL;
		args = parse_args(shell, tokens->content);
		if (args)
		{
			parsed->line = ft_strdup(line);
			parsed->flags = (t_flags){
				.has_stdin = !!tokens->prev, .has_stdout = !!tokens->next
			};
			parsed->redirects = NULL;
			args_loop(shell, args, parsed);
			ft_slstadd_back(&shell->parsed, ft_slstnew(parsed));
			tokens = tokens->next;
		}
		else
			break ;
	}
	ft_lstclear(&tokens_tmp, free);
}

void	fill_data(t_slist *list)
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
