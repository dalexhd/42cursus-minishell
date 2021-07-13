#include "../../includes/minishell.h"

void	sh_error(t_shell *shell, char *s, int force, ...)
{
	va_list	args;

	va_start(args, force);
	shell->status = force;
	ft_vfprintf(STDERR_FILENO, s, &args);
	va_end(args);
}
