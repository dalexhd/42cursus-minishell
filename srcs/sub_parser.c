#include "../includes/minishell.h"

t_aslist	*second_parse(t_shell *shell, t_parsed *parsed, t_aslist *tmp)
{
	t_args	*arg;

	arg = parse_arg(shell, parsed->args, tmp);
	if (parsed->args && arg->cmd[0] == DEL)
	{
		arg->readable = false;
		arg->spaced = ft_alstlast(parsed->args)->content->spaced;
	}
	if (parsed->args && ft_alstlast(parsed->args)
		&& !ft_alstlast(parsed->args)->content->readable)
		arg->spaced = ft_alstlast(parsed->args)->content->spaced;
	ft_alstadd_back(&parsed->args, ft_alstnew(arg));
	return (tmp);
}

void	parse_args(t_shell *shell, t_parsed *parsed, char *cmd)
{
	t_aslist	*tmp;
	t_aslist	*tmplist;
	char		*tmpchar;

	parsed->args = NULL;
	tmpchar = fix_cmd(cmd);
	if (validate_str(shell, tmpchar))
	{
		tmp = ft_safesplitlist_new(tmpchar, ' ', "\"'", true);
		tmplist = tmp;
		while (tmp)
		{
			tmp = second_parse(shell, parsed, tmp);
			tmp = tmp->next;
		}
		ft_aslstclear(&tmplist, free);
		free(tmplist);
	}
}

static	void	exec_commands(t_shell *shell, t_aslist *commands)
{
	if (commands && !shell->force_stop)
	{
		shell->force_stop = false;
		exec_shell(shell, commands->content->arg);
		exec_commands(shell, commands->next);
	}
}

void	parse_commands(t_shell *shell, char *line)
{
	t_aslist	*commands;

	commands = NULL;
	shell->force_stop = false;
	commands = ft_safesplitlist(line, ';', "\"'", false);
	exec_commands(shell, commands);
	ft_aslstclear(&commands, free);
	free(commands);
}

char	*parse_line(t_shell *shell, t_args *arg, char *cmd)
{
	if (cmd)
		return (clean_str(shell, arg, ft_strdup(cmd)));
	return (cmd);
}
