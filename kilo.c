#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>
#include <curses.h>
#include <termcap.h>
#include <term.h>

#define CTRL_KEY(k) ((k) & 0x1f)
#define ABUF_INIT {NULL, 0}
#define KILO_VERSION "0.0.1"
//27
//91 ('[')
//65 ('A')
//27
//91 ('[')
//66 ('B')

/*** TERMINAL ***/

enum editorKey
{
	ARROW_LEFT = 1000,
	ARROW_RIGHT,
	ARROW_UP,
	ARROW_DOWN
};

struct	editorConfig
{
	int	cx;
	int	cy;
	int	screenrows;
	int	screencols;
	struct	termios orig_termios;
};

struct	editorConfig E;

int		ft_putchar(int c)
{
	return (write(1, &c, 1));
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*chain;
	size_t	i;
	size_t	j;
	size_t	len;

	if (!s1 || !s2)
		return (NULL);
	len = (strlen((char*)s1) + strlen((char*)s2) + 1);
	if (!(chain = (char*)malloc(len)))
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i] != '\0')
	{
		chain[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		chain[i + j] = s2[j];
		j++;
	}
	chain[i + j] = '\0';
	return (chain);
}

void	die(const char *s)
{
   	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
	perror(s);
	exit (1);
}

void	disableRawMode()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
		die("tcsetattr");
}

void	enableRawMode()
{
	if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1)//saves config
		die("tcgetattr");
	atexit(disableRawMode);

	struct	termios raw = E.orig_termios;
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)//discards remaining input	
		die("tcsetattr");
}

int	editorReadKey()
{
	int	nread;
	char	seq[4];

	bzero(&seq, 4);
	nread = read(STDIN_FILENO, seq, 3);
	if (nread == 1)
		return (*seq);
	if (strcmp(seq, tgetstr("ku", 0)) == 0)
		return (ARROW_UP);
	else if (strcmp(seq, tgetstr("kd", 0)) == 0)
		return (ARROW_DOWN);
	else if (strcmp(seq, tgetstr("kr", 0)) == 0)
		return (ARROW_RIGHT);
	else if (strcmp(seq, tgetstr("kl", 0)) == 0)
		return (ARROW_LEFT);
	/*
	if (c == '\x1b')
	{
		if (read(STDIN_FILENO, &seq[0], 1) != 1)
			return ('\x1b');
		if (read(STDIN_FILENO, &seq[1], 1) != 1)
			return ('\x1b');
		if  (seq[0] == '[')
		{
			if (seq[1] == 'A')
				return (ARROW_UP);
			else if (seq[1] == 'B')
				return (ARROW_DOWN);
			else if (seq[1] == 'C')
				return (ARROW_RIGHT);
			else if (seq[1] == 'D')
				return (ARROW_LEFT);
		}
		return ('\x1b');
	}
*/
	//else
	//	return (c);
}

int	getCursorPosition(int *rows, int *cols)
{
	char	buf[32];
	unsigned int	i;

	i = 0;

	if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
		return (-1);
	while (i < sizeof(buf) - 1)
	{
		if (read(STDIN_FILENO, &buf[i], 1) != 1)
			break;
		if (buf[i] == 'R')
			break;
		i++;
	}
	buf[i] = '\0';
	if (buf[0] != '\x1b' || buf[1] != '[')
		return (-1);
	if (sscanf(&buf[2], "%d;%d", rows, cols) != 2)
		return (-1);
	return (0);
	//printf("\r\n&buf[1]: '%s'\r\n", &buf[1]);
	/*	printf("\r\n");
	while (read(STDIN_FILENO, &c, 1) == 1)
	{
		if (iscntrl(c))
			printf("%d\r\n", c);
		else
			printf("%D ('%c')\r\n", c, c);
	}
*/	//editorReadKey();
	//return (-1);
}

int	getWindowSize(int *rows, int *cols)
{
	struct	winsize ws;

//	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
	{
		if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12)
			return (-1);
		return (getCursorPosition(rows, cols));
	}
	else
	{
		*cols = ws.ws_col;
		*rows = ws.ws_row;
		return (0);
	}
}

/*** APPEND BUFFER ***/

struct	abuf
{
	char	*b;
	int		len;
};

void	abAppend(struct abuf *ab, const char  *s, int len)
{
	char	*new;

	new = realloc(ab->b, ab->len + len);
	if (new == NULL)
	return ;
	memcpy(&new[ab->len], s, len);
	ab->b = new;
	ab->len += len;
}

void	abFree(struct abuf *ab)
{
	free(ab->b);
}

/*** OUTPUT ***/

void	editorDrawRows(struct abuf  *ab)
{
	int	y;
	int	welcomelen;
	int	padding;
	char	welcome[79];
	y = 0;
	while (y < E.screenrows)
	{
		if (y == E.screenrows / 3)
		{
			welcomelen = snprintf(welcome, sizeof(welcome), "Hellshell -- version %s", KILO_VERSION);
			if (welcomelen > E.screencols)
				welcomelen = E.screencols;
			padding = (E.screencols - welcomelen) / 2;
			if (padding)
			{
				abAppend(ab, "~", 1);
				padding--;
			}
			while (padding--)
				abAppend(ab, " ", 1);
			abAppend(ab, welcome, welcomelen);
		}
		else
			abAppend(ab, "~", 1);
		abAppend(ab, "\x1b[K", 3);
		if (y < E.screenrows -1)
			abAppend(ab, "\r\n", 2);
		y++;
	}
/*	while (y < E.screenrows)
	{
		write(STDOUT_FILENO, "~", 1);
		if (y < E.screenrows - 1)
			write(STDOUT_FILENO, "\r\n", 2);
		y++;
	}
*/
}

void	editorRefreshScreen()
{
	char	buf[32];
	struct	abuf ab = ABUF_INIT;

	abAppend(&ab, "\x1b[?25l", 6);
//	abAppend(&ab, "\x1b[2J", 4);
	abAppend(&ab, "\x1b[H", 3);
	editorDrawRows(&ab);
	snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);
	abAppend(&ab, buf, strlen(buf));
//	abAppend(&ab, "\x1b[H", 3);
	abAppend(&ab, "\x1b[?25h", 6);
	write(STDOUT_FILENO, ab.b, ab.len);
	abFree(&ab);
/*	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
	editorDrawRows();
	write(STDOUT_FILENO, "\x1b[h", 3);
*/
}

/*** INPUT ***/

void	editorMoveCursor(int key)
{
	if (key == ARROW_LEFT)
	{
		if (E.cx != 0)
			E.cx--;
	}
	else if (key == ARROW_RIGHT)
	{
		if (E.cx != E.screencols -1)
			E.cx++;
	}
	else if (key == ARROW_UP)
	{
		if (E.cy != 0)
			E.cy--;
	}
	else if (key == ARROW_DOWN)
	{
		if (E.cy != E.screenrows - 1)
			E.cy++;
	}
}

void	editorProcessKeypress()
{
	int	c;

	c = editorReadKey();
	if (c == CTRL_KEY('q'))
	{
		write(STDOUT_FILENO, "\x1b[2J", 4);
		write(STDOUT_FILENO, "\x1b[H", 3);
		exit(0);
	}
	else if (c == ARROW_LEFT || c == ARROW_RIGHT || c == ARROW_DOWN || c == ARROW_UP)
		editorMoveCursor(c);
}


/*** INIT ***/

void	initEditor()
{
	E.cx = 0;
	E.cy = 0;
	if (getWindowSize(&E.screenrows, &E.screencols) == -1)
		die ("getWindowSize");
}

int	main(void)
{
	enableRawMode();
	initEditor();

	tgetent(NULL, getenv("TERM"));
	tputs(tgetstr("ks", 0), 1, ft_putchar);
	while (1)
	{
		editorRefreshScreen();
		editorProcessKeypress();
	}
	return (0);
}

/*
	while (1)
	{
		char c = '\0';
		if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
			die("read");
		if (iscntrl(c))
			printf("%d\r\n", c);
		else
			printf("%d ('%c')\r\n", c, c);
		if (c == CTRL_KEY('q'))
			break;
	}
*/