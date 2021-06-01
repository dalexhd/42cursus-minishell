#include "../../includes/minishell.h"

static	char	*ft_pwd_sec(char *path, size_t size)
{
	path = ft_calloc(size, 1);
	if (getcwd(path, size) == NULL && errno == ERANGE)
		return (ft_pwd_sec(realloc(path, size + PATH_MAX), size + PATH_MAX));
	return (ft_strdup(path));
}

/*
* Get current path directory
*/
char	*ft_pwd(void)
{
	return (ft_pwd_sec(NULL, PATH_MAX));
}
