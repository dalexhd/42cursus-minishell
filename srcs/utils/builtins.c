#include "../includes/minishell.h"

t_bool	ft_isbuiltin(char *builtin)
{
	return (!ft_strcmp(builtin, "cd")
		|| !ft_strcmp(builtin, "env")
		|| !ft_strcmp(builtin, "export")
		|| !ft_strcmp(builtin, "pwd")
		|| !ft_strcmp(builtin, "exit")
		|| !ft_strcmp(builtin, "unset")
		|| !ft_strcmp(builtin, "echo"));
}

char	*builtin_bin_path(t_shell *shell, char *builtin)
{
	char	**folders;
	int		i;
	char	*path;
	char	*path_env;

	if (file_exists(builtin))
		return (builtin);
	path_env = ft_getenv(shell, "PATH");
	folders = ft_split(path_env, ':');
	free(path_env);
	i = 0;
	while (folders[i] != NULL)
	{
		path = ft_strjoin(folders[i], ft_strjoin("/", builtin));
		if (file_exists(path))
		{
			ft_split_del(folders);
			return (path);
		}
		i++;
	}
	ft_split_del(folders);
	return (NULL);
}
