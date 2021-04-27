#include <stdio.h>
#include <unistd.h>

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	c;
	int	buf;
	int	i;
	int	bwr;

	buf = 1024;

	while (1)
	{
		printf("voidshell%% ");
		
	}	
/*	while  (1)
	{
		printf("voidshell%% ");
		bwr = read(0, line, buf);
		line[bwr] = '\0';
//		printf("%s", line);
	}
*/
	return (0);
}

