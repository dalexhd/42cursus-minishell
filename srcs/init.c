#include "../includes/minishell.h"

static void	fill_redirect(t_rstatus *status, t_alist *args)
{
	if (args->content->type == R_IN)
	{
		status->fd = open(status->file, O_RDONLY, 0600);
		if (status->fd < 0)
			ft_error("minishell: %s: %s\n", false, status->file,
				strerror(errno));
	}
	else if (args->content->type == R_OUT)
	{
		status->fd = open(status->file, O_TRUNC | O_WRONLY | O_CREAT, 0644);
		if (status->fd < 0)
			ft_error("minishell: %s: %s\n", false, status->file,
				strerror(errno));
	}
	else if (args->content->type == R_AOUT)
	{
		status->fd = open(status->file, O_WRONLY | O_CREAT | O_APPEND, 0600);
		if (status->fd < 0)
			ft_error("minishell: %s: %s\n", false, status->file,
				strerror(errno));
	}
}

static int	split(t_redirect *redirect, t_alist *args,
	t_parsed *parsed, t_rstatus *status)
{
	status->status = true;
	if (!args->next)
	{
		ft_error("minishell: syntax error near unexpected token `newline'\n",
			false);
		parsed->valid = false;
		return (1);
	}
	status->file = ft_strdup(args->next->content->file);
	fill_redirect(status, args);
	args->content->readable = false;
	args->next->content->readable = false;
	ft_rlstadd_back(&parsed->redirects, ft_rlstnew(redirect));
	return (0);
}

static	void	args_loop(t_shell *shell, t_alist *args, t_parsed *parsed)
{
	t_redirect	*red;
	t_rstatus	rs;

	if (args && args->content)
	{
		if (args->content->type == R_IN || args->content->type == R_OUT
			|| args->content->type == R_AOUT)
		{
			red = (t_redirect *)malloc(sizeof(t_redirect));
			rs = (t_rstatus){.status = false, .file = NULL};
			red->in = rs;
			red->out = rs;
			red->aout = rs;
			if (args->content->type == R_IN)
				shell->status = split(red, args, parsed,
						&red->in);
			else if (args->content->type == R_OUT)
				shell->status = split(red, args, parsed, &red->out);
			else if (args->content->type == R_AOUT)
				shell->status = split(red, args, parsed, &red->aout);
		}
	}
	if (args->next)
		args_loop(shell, args->next, parsed);
}

void	lsh_split_line(t_shell *shell, char *line)
{
	t_aslist	*tokens;
	t_aslist	*tokens_tmp;
	t_parsed	*parsed;

	if (line[0] == '|')
	{
		ft_error("minishell: syntax error near unexpected token `|'\n", 0);
		shell->status = 1;
		return ;
	}
	tokens = ft_safesplitlist(line, '|', "\"'", false);
	tokens_tmp = tokens;
	while (tokens)
	{
		parsed = (t_parsed *)malloc(sizeof(t_parsed));
		parsed->valid = true;
		parsed->line = NULL;
		ft_slstadd_back(&shell->parsed, ft_slstnew(parsed));
		parse_args(shell, &parsed, tokens->content->arg);
		if (shell->parsed->content->args)
		{
			parsed->line = ft_strdup(line);
			parsed->redirects = NULL;
			args_loop(shell, parsed->args, parsed);
			tokens = tokens->next;
		}
		else
			break ;
	}
	ft_aslstclear(&tokens_tmp, free);
}
