#include "../includes/minishell.h"

void	parse_args(t_shell *shell, t_parsed **parsed, char *cmd)
{
	t_args		*arg;
	t_aslist	*tmp;
	t_aslist	*tmplist;
	char		*tmpchar;

	(*parsed)->args = NULL;
	tmpchar = fix_cmd(cmd);
	if (validate_str(shell, tmpchar))
	{
		tmp = ft_safesplitlist_new(tmpchar, ' ', "\"'", true);
		tmplist = tmp;
		while (tmp)
		{
			arg = parse_arg(shell, (*parsed)->args, tmp);
			if ((*parsed)->args && arg->cmd[0] == DEL)
			{
				arg->readable = false;
				arg->spaced = ft_alstlast((*parsed)->args)->content->spaced;
			}
			if ((*parsed)->args && ft_alstlast((*parsed)->args)
				&& !ft_alstlast((*parsed)->args)->content->readable)
				arg->spaced = ft_alstlast((*parsed)->args)->content->spaced;
			ft_alstadd_back(&(*parsed)->args, ft_alstnew(arg));
			tmp = tmp->next;
		}
		ft_aslstclear(&tmplist, free);
	}
}

void	parse_commands(t_shell *shell, char *line)
{
	t_aslist	*commands;
	t_aslist	*commands_tmp;

	commands = ft_safesplitlist(line, ';', "\"'", false);
	commands_tmp = commands;
	shell->force_stop = false;
	while (commands && !shell->force_stop)
	{
		exec_shell(shell, commands->content->arg);
		commands = commands->next;
	}
	ft_aslstclear(&commands_tmp, free);
}

char	*parse_line(t_shell *shell, t_args *arg, char *cmd)
{
	if (cmd)
		return (clean_str(shell, arg, ft_strdup(cmd)));
	return (cmd);
}
