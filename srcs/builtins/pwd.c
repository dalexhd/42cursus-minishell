#include "../../includes/minishell.h"

static	char	*ft_pwd_sec(char *path, size_t size)
{
	if (path)
		free(path);
	path = ft_calloc(size, sizeof(char));
	if (getcwd(path, size) == NULL && errno == ERANGE)
		return (ft_pwd_sec(path, size + PATH_MAX));
	return (path);
}

/**
 * Get current path directory
**/
char	*ft_pwd(void)
{
	return (ft_pwd_sec(NULL, PATH_MAX));
}
