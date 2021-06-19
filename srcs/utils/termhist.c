#include "../../includes/minishell.h"

void	new(t_shell *shell)
{
	int		input;
	char	*tmp_dir;

	tmp_dir = ft_getenv(shell, "HOME");
	if (tmp_dir)
	{
		input = open(ft_strjoin(tmp_dir, "/.minishell_history"),
				O_CREAT | O_WRONLY | O_APPEND, 0600);
		write(input, ft_strjoin(shell->term.line, "\n"),
			ft_strlen(shell->term.line) + 1);
		close(input);
	}
}

void	old(t_shell *shell)
{
	int		test;
	int		fd;
	char	*line;
	char	*tmp_dir;

	tmp_dir = ft_getenv(shell, "HOME");
	if (tmp_dir)
	{
		fd = open(ft_strjoin(tmp_dir, "/.minishell_history"),
				O_CREAT | O_RDONLY, 0600);
		test = get_next_line(fd, &line);
		while (test)
		{
			ft_hlstadd_front(&shell->term.history, ft_hlstnew(ft_strdup(line)));
			shell->term.history->original = ft_strdup(line);
			shell->term.history->copy = ft_strdup(line);
			test = get_next_line(fd, &line);
		}
		close(fd);
	}
}

void	history_up(t_shell *shell)
{
	if (shell->term.history->next)
	{
		tputs(tgetstr("cr", NULL), 1, ft_iputchar);
		tputs(tgetstr("dl", NULL), 1, ft_iputchar);
		ft_printshell(shell);
		free(shell->term.history->copy);
		shell->term.history->copy = ft_strdup(shell->term.line);
		shell->term.history = shell->term.history->next;
		ft_strcpy(shell->term.line, shell->term.history->copy);
		ft_putstr_fd(shell->term.history->copy, 1);
		shell->term.pos = ft_strlen(shell->term.history->copy);
		shell->term.cursor = 11 + shell->term.pos;
	}
}

void	history_down(t_shell *shell)
{
	if (!shell->term.history)
		return ;
	if (shell->term.history->prev)
	{
		tputs(tgetstr("cr", NULL), 1, ft_iputchar);
		tputs(tgetstr("dl", NULL), 1, ft_iputchar);
		ft_printshell(shell);
		free(shell->term.history->copy);
		shell->term.history->copy = ft_strdup(shell->term.line);
		shell->term.history = shell->term.history->prev;
		ft_strcpy(shell->term.line, shell->term.history->copy);
		ft_putstr_fd(shell->term.line, 1);
		shell->term.pos = ft_strlen(shell->term.line);
		shell->term.cursor = 11 + shell->term.pos;
	}
}
