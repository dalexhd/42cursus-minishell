#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>

#define CTRL_KEY(k) ((k) & 0x1f)
//27
//91 ('[')
//65 ('A')
//27
//91 ('[')
//66 ('B')

/*** TERMINAL ***/

struct	editorConfig
{
	int	screenrows;
	int	screencols;
	struct	termios orig_termios;
};

struct	editorConfig E;

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
//	raw.c_cc[VMIN] = 0;
//	raw.c_cc[VTIME] = 1;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)//discards remaining input	
		die("tcsetattr");
}

char	editorReadKey()
{
	int	nread;
	char	c;

	while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
	{
		if (nread == -1 && errno != EAGAIN)
			die("read");
	}
	return (c);
}

int	getCursorPosition(int *rows, int *cols)
{
	char	c;

	if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
		return (-1);
	printf("\r\n");
	while (read(STDIN_FILENO, &c, 1) == 1)
	{
		if (iscntrl(c))
			printf("%d\r\n", c);
		else
			printf("%D ('%c')\r\n", c, c);
	}
	editorReadKey();
	return (-1);
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

/*** OUTPUT ***/

void	editorDrawRows()
{
	int	y;

	y = 0;
	while (y < E.screenrows)
	{
		write(STDOUT_FILENO, "~\r\n", 3);
		y++;
	}
	write(STDOUT_FILENO, "voidshell~ ", 12);
}

void	editorRefreshScreen()
{
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
	editorDrawRows();
	write(STDOUT_FILENO, "\x1b[h", 3);
}

/*** INPUT ***/

void	editorProcessKeypress()
{
	char	c;

	c = editorReadKey();
	if (c == CTRL_KEY('q'))
	{
		exit(0);
	}
}


/*** INIT ***/

void	initEditor()
{
	if (getWindowSize(&E.screenrows, &E.screencols) == -1)
		die ("getWindowSize");
}

int	main(void)
{
	enableRawMode();
	initEditor();
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
