/*
** pipex.c - multipipes support
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


/*
 * loop over commands by sharing
 * pipes.
 */
static void	pipeline(char ***cmd)
{
	int		fd[2];
	pid_t	pid;
	int		fdd;

	fdd = 0;	/* Backup */
	while (*cmd != NULL)
	{
		pipe(fd);
		if ((pid = fork()) == -1)
		{
			perror("fork");
			exit(1);
		}
		else if (pid == 0)
		{
			printf("Padre %d\n", pid);
			dup2(fdd, 0);
			if (*(cmd + 1) != NULL)
				dup2(fd[1], 1);
			close(fd[0]);
			execvp((*cmd)[0], *cmd);
			exit(1);
		}
		else /* Collect childs */
		{
			wait(NULL);
			printf("Hijo: %s %d\n", **cmd, pid);
			close(fd[1]);
			fdd = fd[0];
			cmd++;
		}
	}
}

/*
 * Compute multi-pipeline based
 * on a command list.
 */
int	main(int argc, char *argv[])
{
	char *cat[] = {"cat", "/dev/urandom", NULL};
	char *less[] = {"less", NULL};
	char **cmd[] = {cat, less, NULL};

	pipeline(cmd);
	return (0);
}
