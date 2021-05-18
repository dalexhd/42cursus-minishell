#include <termcap.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include <term.h>
#include <errno.h>

//printf, malloc, free, write, open, read, close,
//fork, wait, waitpid, wait3, wait4, signal, kill,
//exit, getcwd, chdir, stat, lstat, fstat, execve,
//dup, dup2, pipe, opendir, readdir, closedir,
//strerror, errno, isatty, ttyname, ttyslot, ioctl,
//getenv, tcsetattr, tcgetattr, tgetent, tgetflag,
//tgetnum, tgetstr, tgoto, tputs

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
	struct s_list	*prev;
}				t_list;

typedef struct s_config
{
	char			*term_name;
	char			line[2048];
	t_list			*text;
	int				pos;
	int				cursor;
	struct termios	termios_raw;
	struct termios	termios_new;
}				t_config;

void	ft_lstadd_front(t_list **alst, t_list *new)
{
	if (*alst)
	{
		new->next = *alst;
		(*alst)->prev = new;
	}
	*alst = new;
}

t_list	*ft_lstnew(void *content)
{
	t_list	*elem;

	elem = (t_list *)malloc(sizeof(t_list));
	if (elem == NULL)
		return (NULL);
	elem->content = content;
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
	size = strlen(src);
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

void	end_tc(t_config *e)
{
	write(STDOUT_FILENO, "end tc\n", 7);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &e->termios_raw);
	exit(1);
}

void	init_tc(t_config *e)
{
	bzero(e, sizeof(t_config));
	e->cursor  = 12;
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
	if (e->cursor > 12)
	{
		tputs(tgetstr("le", NULL), 1, ft_putchar);
		tputs(tgetstr("dc", NULL), 1, ft_putchar);
		e->cursor--;
		e->line[--e->pos] = 0;
	}
}

void	uplore(t_config *e)
{
//	write(STDOUT_FILENO, "historial arriba", 17);
	if (!e->text)
		return ;
	tputs(tgetstr("cr", NULL), 1, ft_putchar);
	tputs(tgetstr("dl", NULL), 1, ft_putchar);
	ft_putstr_fd("hellshell$ ", 1);
	ft_putstr_fd(e->text->content, 1);
	strlcpy(e->line, e->text->content, strlen(e->text->content));
	e->cursor = 12 + strlen(e->text->content);
	if (e->text->next)
		e->text = e->text->next;
}

void	downlore(t_config *e)
{
	if (!e->text)
		return ;
	if (e->text->prev)
	{
		tputs(tgetstr("cr", NULL), 1, ft_putchar);
		tputs(tgetstr("dl", NULL), 1, ft_putchar);
//		write(STDOUT_FILENO, "hellshell$  ", 11);
		ft_putstr_fd("hellshell$ ", 1);
		ft_putstr_fd(e->text->prev->content, 1);
		strlcpy(e->line, e->text->content, strlen(e->text->content));
		e->cursor = 12 + strlen(e->text->prev->content);
		e->text = e->text->prev;
	}
}

void	sandman(t_config *e)
{
//	write(STDOUT_FILENO, "\nlinea-0: ", 10);
//	ft_putstr_fd(e->line, 1);
	if (*e->line)
		ft_lstadd_front(&e->text, ft_lstnew(strdup(e->line)));
	bzero(&e->line, 2048);
	e->pos = 0;
	write(STDOUT_FILENO, "\nhellshell$ ", 13);
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
	bzero(&buf, 4);
	while (read(STDIN_FILENO, &buf, 4) > 0)
	{
		if (buf[0] == 'D' - 64)//es un puto exit
			end_tc(e);
		else if (buf[0] == 127)
			eraser(e);
		else if (buf[0] == '\\' - 64)//limpia la linea
			end_tc(e);
		else if (buf[0] == 'C' - 64)//es como un enter perro sin historial
			tputs(tgetstr("cl", NULL), 1, ft_putchar);
		else if (!strcmp(buf, tgetstr("ku", NULL)))
			uplore(e);
		else if (!strcmp(buf, tgetstr("kd", NULL)))
			downlore(e);
		else if (buf[0] == 'M' - 64)
			sandman(e);
		else if (buf[0] > 31 && buf[0] < 127 && buf[1] == 0)
			tear(e, buf[0]);
		bzero(&buf, 4);
	}
}

int	main(int arc, char **arv, char **env)
{
	t_config	e;

	init_tc(&e);
	write(STDOUT_FILENO, "hellshell$ ", 12);
	loureed(&e);
	end_tc(&e);
	return (0);
}

/*
void	sandman(t_config *e)
{
	write(STDOUT_FILENO, "\nlinea-0: ", 10);
	ft_putstr_fd(e->text[0], 1);
	write(STDOUT_FILENO, "\nlinea-1: ", 10);
	ft_putstr_fd(e->text[1], 1);
	bzero(e->text[1], 2048);
	ft_strlcpy(e->text[1], e->text[0], 2048);
	bzero(e->text[0], 2048);
	write(STDOUT_FILENO, "\n", 1);
	e->pos = 0;
}
void	tear(t_config *e, char c)
{
	write(STDOUT_FILENO, &c, 1);
	e->text[0][e->pos++] = c;
	e->text[0][e->pos] = 0;
//	write(STDOUT_FILENO, &e->text[e->pos], 1);
//	ft_putstr_fd(e->text, 1);
//	write(STDOUT_FILENO, "\n", 1);
}
*/
