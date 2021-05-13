#include "includes/minishell.h"
#include <stdio.h>
#include <time.h>


void ft_clear_memory(t_shell *shell)
{
	int i;

	i = 0;
	while (shell->envp[i] != 0)
	{
		//ft_printf("%s\n", shell.envp[i]);
		//free(shell.envp[i]);
		i++;
	}
}

int timediff(clock_t t1, clock_t t2)
{
	long elapsed;
	elapsed =  ((double)t2 - t1) / CLOCKS_PER_SEC * 1000;
	return elapsed;
}


int main(int argc, char **argv, char **envp)
{
	t_shell *shell;

	(void)argc;
	(void)**argv;
	shell = init_shell(argv[1], envp);
	while (shell->running)
	{
		if (shell->elapsed)
			ft_printf(C_GREEN "%s "C_X"(%ims)" C_GREEN "❯ " C_X, getCurrentDir(ft_pwd()), shell->elapsed);
		else
			ft_printf(C_CYAN "Wellcome to our minishell 😋" C_GREEN "❯ " C_X, shell->elapsed);
		fflush(stdout);
		clock_t t1, t2;
		shell->elapsed = 0;
		t1 = clock();
		char input[1024];
		fgets(input, 1024, stdin);
		exec_shell(shell, input);
		t2 = clock();
		shell->elapsed = timediff(t1, t2);
	}
	ft_clear_memory(shell);
	ft_success("Nice 🧡", 1);
	return (0);
}
