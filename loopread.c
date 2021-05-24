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
	void			*cop;
	struct s_list	*next;
	struct s_list	*prev;
}				t_list;

typedef struct s_config
{
	char			*term_name;
	char			line[2048];
	char			aux[2048];
	t_list			*text;
	int				pos;
	int				cursor;
	struct termios	termios_raw;
	struct termios	termios_new;
}				t_config;

t_list	*ft_lstfirst(t_list *lst)
{
	if (!lst)
		return (0);
	while (lst->prev != 0)
	{
		lst = lst->prev;
	}
	return (lst);
}

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
	elem->cop = content;
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
	bzero(&e->line, 2048);
	e->pos = 0;
	write(STDOUT_FILENO, "\nhellshell$ ", 13);
	e->cursor = 11;
}

void	ctlb(t_config *e)
{
	bzero(&e->line, 2048);
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
	bzero(e, sizeof(t_config));
	e->cursor  = 11;
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

void	edition(t_config *e)
{
	strcpy(e->aux, e->line);
}

void	eraser(t_config *e)
{
	if (e->cursor > 11)
	{
		tputs(tgetstr("le", NULL), 1, ft_putchar);
		tputs(tgetstr("dc", NULL), 1, ft_putchar);
		e->cursor--;
		e->line[--e->pos] = 0;
		edition(e);
	}
}

void	uplore(t_config *e)
{
	if (!e->text)
		return ;
	tputs(tgetstr("cr", NULL), 1, ft_putchar);
	tputs(tgetstr("dl", NULL), 1, ft_putchar);
	ft_putstr_fd("hellshell$ ", 1);
	strcpy(e->line, e->text->cop);
	ft_putstr_fd(e->text->cop, 1);
	e->pos = strlen(e->text->cop);
	e->cursor = 11 + e->pos;
	if (e->text->next)
		e->text = e->text->next;
}

//cargar historial en line /////
//guardar line en aux/////
//mostrar aux al volver al inicio de historial/////
//comprobar que la instrucion no se repite/////
//apuntar a principio al pulsar enter/////
//la modificacion del historial se mantiene mientras navegas
//

void	downlore(t_config *e)
{
	if (!e->text)
		return ;
	if (e->text->prev)
	{
		tputs(tgetstr("cr", NULL), 1, ft_putchar);
		tputs(tgetstr("dl", NULL), 1, ft_putchar);
		ft_putstr_fd("hellshell$ ", 1);
		strcpy(e->line, e->text->prev->cop);
		ft_putstr_fd(e->text->prev->cop, 1);
		e->pos = strlen(e->text->prev->cop);
		e->cursor = 11 + e->pos;
		e->text = e->text->prev;
	}
	else
	{
		tputs(tgetstr("cr", NULL), 1, ft_putchar);
		tputs(tgetstr("dl", NULL), 1, ft_putchar);
		ft_putstr_fd("hellshell$ ", 1);
		strcpy(e->line, e->aux);
		ft_putstr_fd(e->line, 1);
		e->pos = strlen(e->line);
//		e->pos = 0;
		e->cursor = 11 + e->pos;
	}
}

void	sandman(t_config *e)
{
	if (*e->line)
	{
		e->line[e->pos] = 0;
		e->text = ft_lstfirst(e->text);
		if  (!e->text)
			ft_lstadd_front(&e->text, ft_lstnew(strdup(e->line)));
		else
			if (strncmp(e->text->cop, e->line, strlen(e->text->cop)) != 0)
				ft_lstadd_front(&e->text, ft_lstnew(strdup(e->line)));
	}
	bzero(&e->line, 2048);
	bzero(&e->aux, 2048);
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
	edition(e);
}

void	loureed(t_config *e)
{
	char	buf[4];

	e->pos = 0;
	bzero(&buf, 4);
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
