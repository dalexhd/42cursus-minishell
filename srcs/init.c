#include "../includes/minishell.h"

static int	fill_redirect(t_parsed *parsed, t_rstatus *status, t_alist *args)
{
	if (args->content->type == R_IN)
		status->fd = open(status->file, O_RDONLY, 0600);
	else if (args->content->type == R_OUT)
		status->fd = open(status->file, O_TRUNC | O_WRONLY | O_CREAT, 0666);
	else if (args->content->type == R_AOUT)
		status->fd = open(status->file, O_WRONLY | O_CREAT | O_APPEND, 0600);
	if (status->fd < 0)
		ft_error(ERR_RED, false, status->file, strerror(errno));
	status->status = status->fd != -1;
	parsed->valid = status->status;
	if (!status->status)
		ft_strdel(&status->file);
	return (parsed->valid == 0);
}

static int	split(t_redirect *redirect, t_alist *args,
	t_parsed *parsed, t_rstatus *status)
{
	status->status = true;
	ft_rlstadd_back(&parsed->redirects, ft_rlstnew(redirect));
	if (!args->next)
	{
		ft_error(ERR_UT, false, "newline");
		parsed->valid = false;
		return (1);
	}
	status->file = ft_strdup(args->next->content->file);
	args->content->readable = false;
	args->next->content->readable = false;
	return (fill_redirect(parsed, status, args));
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
	if (args && args->next)
		args_loop(shell, args->next, parsed);
}

void	lsh_split_line(t_shell *shell, char *line)
{
	t_aslist	*tokens;
	t_aslist	*tokens_tmp;
	t_parsed	*parsed;

	if (!valid_pipes(shell, line))
		return ;
	g_running = '|';
	tokens = ft_safesplitlist(line, '|', "\"'");
	g_running = true;
	tokens_tmp = tokens;
	while (tokens)
	{
		parsed = (t_parsed *)malloc(sizeof(t_parsed));
		ft_slstadd_back(&shell->parsed, ft_slstnew(parsed));
		parse_args(shell, parsed, &tokens->content->arg);
		if (shell->parsed->content->args)
		{
			parsed->line = ft_strdup(line);
			args_loop(shell, parsed->args, parsed);
		}
		tokens = tokens->next;
	}
	ft_aslstclear(&tokens_tmp, free);
}

void	shell_lvl(t_shell *shell)
{
	t_env	*env;
	char	*tmp;

	env = ft_getenv(shell, "SHLVL", true);
	if (env && ft_strevery(env->val, ft_isalnum))
	{
		tmp = ft_itoa(ft_atoi(env->val) + 1);
		ft_export_internal(shell, "SHLVL", tmp);
		ft_strdel(&tmp);
	}
	else
		ft_export_internal(shell, "SHLVL", "1");
}
