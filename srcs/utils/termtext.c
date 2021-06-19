#include "../../includes/minishell.h"

void	eraser(t_shell *shell)
{
	if (shell->term.cursor > 11)
	{
		tputs(tgetstr("le", NULL), 1, ft_iputchar);
		tputs(tgetstr("dc", NULL), 1, ft_iputchar);
		shell->term.cursor--;
		shell->term.line[--shell->term.pos] = 0;
	}
}

void	newliner(t_shell *shell)
{
	if (*shell->term.line)
	{
		shell->term.history->copy = ft_strdup(shell->term.history->original);
		shell->term.history = ft_hlstfirst(shell->term.history);
		free(shell->term.history->original);
		free(shell->term.history->copy);
		shell->term.history->original = ft_strdup(shell->term.line);
		shell->term.history->copy = ft_strdup(shell->term.line);
		new (shell);
		ft_hlstadd_front(&shell->term.history, ft_hlstnew(ft_strdup("")));
	}
}

void	sandman(t_shell *shell)
{
	t_list	*commands;

	newliner(shell);
	free(shell->term.history->copy);
	shell->term.history->copy = ft_strdup(shell->term.history->original);
	ft_putchar_fd('\n', STDOUT_FILENO);
	commands = ft_safesplitlist(shell->term.line, ';', "\"'");
	while (commands)
	{
		exec_shell(shell, commands->content);
		commands = commands->next;
	}
	tputs(tgetstr("ks", NULL), 1, ft_iputchar);
	ft_bzero(&shell->term.line, BUFF_SIZE);
	shell->term.history = ft_hlstfirst(shell->term.history);
	shell->term.pos = 0;
	if (g_running)
		ft_printshell(shell);
	shell->term.cursor = 11;
}

void	tear(t_shell *shell, char c)
{
	write(STDOUT_FILENO, &c, 1);
	shell->term.line[shell->term.pos++] = c;
	shell->term.line[shell->term.pos] = 0;
	shell->term.cursor++;
}
