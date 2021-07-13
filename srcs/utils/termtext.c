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
		free(shell->term.history->copy);
		shell->term.history->copy = ft_strdup(shell->term.history->original);
		shell->term.history = ft_hlstfirst(shell->term.history);
		free(shell->term.history->original);
		free(shell->term.history->copy);
		shell->term.history->original = ft_strdup(shell->term.line);
		shell->term.history->copy = ft_strdup(shell->term.line);
		new(shell);
		ft_hlstadd_front(&shell->term.history, ft_hlstnew(""));
	}
}

void	sandman(t_shell *shell)
{
	newliner(shell);
	free(shell->term.history->copy);
	shell->term.history->copy = ft_strdup(shell->term.history->original);
	ft_putchar_fd('\n', STDOUT_FILENO);
	if (shell->term.line[0] == ';')
	{
		ft_error("minishell: syntax error near unexpected token `;'\n", 0);
		shell->status = 1;
	}
	else
		parse_commands(shell, shell->term.line);
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
