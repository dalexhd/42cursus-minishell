#include "../../includes/minishell.h"

void	debug_log(char *string)
{
	char	*file;
	int		input;

	file = "debug.log";
	input = open(file, O_WRONLY | O_CREAT | O_APPEND, 0600);
	if (input < 0)
		return (ft_error("minishell: %s: %s\n", false, file, strerror(errno)));
	ft_fprintf(input, string);
	close(input);
}
