#include <termcap.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include <term.h>
#include <errno.h>

typedef struct s_hist
{
	void			*original;
	void			*copy;
	struct s_hist	*next;
	struct s_hist	*prev;
}				t_hist;

typedef struct s_config
{
	char			*term_name;
	char			line[2048];
	t_hist			*hist;
	int				pos;
	int				cursor;
	struct termios	termios_raw;
	struct termios	termios_new;
}				t_config;

t_hist	*ft_hstfirst(t_hist *hst)
{
	if (!hst)
		return (0);
	while (hst->prev != 0)
	{
		hst = hst->prev;
	}
	return (hst);
}

void	ft_hstadd_front(t_hist **ahst, t_hist *new)
{
	if (*ahst)
	{
		new->next = *ahst;
		(*ahst)->prev = new;
	}
	*ahst = new;
}

t_hist	*ft_hstnew(void *content)
{
	t_hist	*elem;

	elem = (t_hist *)malloc(sizeof(t_hist));
	if (elem == NULL)
		return (NULL);
	elem->original = ft_strdup(content);
	elem->copy = ft_strdup(content);
	elem->next = NULL;
	elem->prev = NULL;
	return (elem);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t			i;
	size_t			size;
	unsigned char	*source;

	if (!dst)
		return (dstsize);
	size = ft_strlen(src);
	i = 0;
	source = (unsigned char *)src;
	if (dstsize > 0)
	{
		while (src[i] != '\0' && i < dstsize - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (size);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	if (s)
	{
		i = 0;
		while (s[i] != '\0')
		{
			write(fd, &s[i], sizeof(s[i]));
			i++;
		}
	}
}

int	ft_putchar(int c)
{
	return (write(1, &c, 1));
}

void	die(char *msg)
{
	perror(msg);
	exit (1);
}

void	ctld(t_config *e)
{
	if (!*e->line)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		exit(0);
	}
}

void	ctlc(t_config *e)
{
	ft_bzero(&e->line, 2048);
	e->pos = 0;
	write(STDOUT_FILENO, "\nhellshell$ ", 12);
	e->cursor = 11;
}

void	ctlb(t_config *e)
{
	ft_bzero(&e->line, 2048);
	e->pos = 0;
	tputs(tgetstr("cr", NULL), 1, ft_putchar);
	tputs(tgetstr("dl", NULL), 1, ft_putchar);
	write(STDOUT_FILENO, "hellshell$ ", 11);
	e->cursor = 11;
}

void	end_tc(t_config *e)
{
	write(STDOUT_FILENO, "end tc\n", 7);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &e->termios_raw);
	exit(1);
}

void	init_tc(t_config *e)
{
	ft_bzero(e, sizeof(t_config));
	e->cursor = 11;
	ft_hstadd_front(&e->hist, ft_hstnew(ft_strdup("")));
	e->term_name = getenv("TERM");
	tgetent(NULL, e->term_name);
	if (tcgetattr(STDIN_FILENO, &e->termios_raw) == -1)
		die("tcgetattr");
	e->termios_new = e->termios_raw;
	e->termios_new.c_iflag &= ~(BRKINT | ICRNL | IXON);
	e->termios_new.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	e->termios_new.c_cc[VMIN] = 1;
	e->termios_new.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &e->termios_new) == -1)
		die("tcsetattr");
	tputs(tgetstr("ks", NULL), 1, ft_putchar);
}

void	eraser(t_config *e)
{
	if (e->cursor > 11)
	{
		tputs(tgetstr("le", NULL), 1, ft_putchar);
		tputs(tgetstr("dc", NULL), 1, ft_putchar);
		e->cursor--;
		e->line[--e->pos] = 0;
	}
}

void	uplore(t_config *e)
{
	if (e->hist->next)
	{
		tputs(tgetstr("cr", NULL), 1, ft_putchar);
		tputs(tgetstr("dl", NULL), 1, ft_putchar);
		ft_putstr_fd("hellshell$ ", 1);
		free(e->hist->copy);
		e->hist->copy = ft_strdup(e->line);
		e->hist = e->hist->next;
		ft_strcpy(e->line, e->hist->copy);
		ft_putstr_fd(e->hist->copy, 1);
		e->pos = ft_strlen(e->hist->copy);
		e->cursor = 11 + e->pos;
	}
}

void	downlore(t_config *e)
{
	if (!e->hist)
		return ;
	if (e->hist->prev)
	{
		tputs(tgetstr("cr", NULL), 1, ft_putchar);
		tputs(tgetstr("dl", NULL), 1, ft_putchar);
		ft_putstr_fd("hellshell$ ", 1);
		free(e->hist->copy);
		e->hist->copy = ft_strdup(e->line);
		e->hist = e->hist->prev;
		ft_strcpy(e->line, e->hist->copy);
		ft_putstr_fd(e->line, 1);
		e->pos = ft_strlen(e->line);
		e->cursor = 11 + e->pos;
	}
}

void	sandman(t_config *e)
{
	if (*e->line)
	{
		e->hist->copy = ft_strdup(e->hist->original);
		e->hist = ft_hstfirst(e->hist);
		free(e->hist->original);
		free(e->hist->copy);
		e->hist->original = ft_strdup(e->line);
		e->hist->copy = ft_strdup(e->line);
		ft_hstadd_front(&e->hist, ft_hstnew(ft_strdup("")));
	}
	free(e->hist->copy);
	e->hist->copy = ft_strdup(e->hist->original);
	ft_bzero(&e->line, 2048);
	e->hist = ft_hstfirst(e->hist);
	e->pos = 0;
	write(STDOUT_FILENO, "\nhellshell$ ", 12);
	e->cursor = 11;
}

void	tear(t_config *e, char c)
{
	write(STDOUT_FILENO, &c, 1);
	e->line[e->pos++] = c;
	e->line[e->pos] = 0;
	e->cursor++;
}

void	loureed(t_config *e)
{
	char	buf[4];

	e->pos = 0;
	ft_bzero(&buf, 4);
	while (read(STDIN_FILENO, &buf, 4) > 0)
	{
		if (buf[0] == 'D' - 64)
			ctld(e);
		else if (buf[0] == 127)
			eraser(e);
		else if (buf[0] == '\\' - 64)
			ctlb(e);
		else if (buf[0] == 'C' - 64)
			ctlc(e);
		else if (!ft_strcmp(buf, tgetstr("ku", NULL)))
			uplore(e);
		else if (!ft_strcmp(buf, tgetstr("kd", NULL)))
			downlore(e);
		else if (buf[0] == 'M' - 64)
			sandman(e);
		else if (buf[0] > 31 && buf[0] < 127 && buf[1] == 0)
			tear(e, buf[0]);
		ft_bzero(&buf, 4);
	}
}

int	main(int arc, char **arv, char **env)
{
	t_config	e;

	init_tc(&e);
	write(STDOUT_FILENO, "hellshell$ ", 11);
	loureed(&e);
	end_tc(&e);
	return (0);
}
