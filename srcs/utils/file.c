#include "../../includes/minishell.h"

t_bool	file_exists(char *filename)
{
	struct stat	buffer;

	return (stat(filename, &buffer) == 0);
}

t_bool	is_directory(char *path)
{
	DIR	*dir;

	if (ft_strcmp(path, "..") == 0)
		return (false);
	dir = opendir(path);
	if (dir)
	{
		closedir(dir);
		return (true);
	}
	else if (ENOENT == errno)
		return (false);
	return (false);
}

t_bool	has_access(char *path)
{
	int	input;

	input = open(path, O_RDONLY);
	if (input < 0 && errno == EACCES)
		return (false);
	return (true);
}

char	*get_current_dir(char *path)
{
	char	*token;
	char	*directory;
	size_t	length;

	token = ft_strrchr(path, '/');
	if (token == NULL)
		ft_error("Error getting dir", 1);
	length = ft_strlen(token);
	directory = ft_strnew(length);
	ft_memcpy(directory, token + 1, length);
	return (directory);
}
