#include <stdio.h>
#include <unistd.h>
#include <string.h>

void	ft_putchar_ft(char c, int fd)
{
	write(fd, &c, sizeof(c));
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

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	*pr;
	char	c;
	int	buf;
	int	i;
	int	bwr;

	while (1)
	{
		bwr = 0;
		i = 0;
		ft_putstr_fd("voidshell%% ", 1);
		bwr = read(1, &c, 1);
		while (bwr && c != '\n')
		{
			line[i++] = c;
			bwr = read(1, &c, 1);
		}
		line[i++] = c;
		line[i++] = '\0';
		ft_putstr_fd(line, 1);//enviar line a pasear
	}
	return (0);
}

