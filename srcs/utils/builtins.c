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

/**
 * Here we exec the builtin
 *
**/
int	ft_exec_builtin(t_shell *shell, t_slist *parsed)
{
	char	**args;
	char	*pwd;

	args = ft_safesplit(shell, parsed->content->args);
	if (!ft_strcmp(parsed->content->args->content->cmd, "cd"))
		ft_cd(shell, args);
	else if (!ft_strcmp(parsed->content->args->content->cmd, "exit"))
		ft_exit(shell, args);
	else if (!ft_strcmp(parsed->content->args->content->cmd, "export"))
		ft_export(shell, args);
	else if (!ft_strcmp(parsed->content->args->content->cmd, "unset"))
		ft_unset(shell, args);
	else if (!ft_strcmp(parsed->content->args->content->cmd, "env"))
		ft_env(shell);
	else if (!ft_strcmp(parsed->content->args->content->cmd, "pwd"))
	{
		pwd = ft_pwd();
		ft_printf("%s\n", pwd);
		ft_strdel(&pwd);
		shell->status = 0;
	}
	else if (!ft_strcmp(parsed->content->args->content->cmd, "echo"))
		ft_echo(args);
	ft_split_del(args);
	return (shell->status);
}

/**
 * Here we exec the system builtin
 *
**/
void	ft_exec_bin(t_shell *shell, t_alist *args)
{
	char	**args_split;
	t_args	*arg;

	arg = args->content;
	args_split = ft_safesplit(shell, args);
	if (!args_split[0])
	{
		ft_split_del(args_split);
		exit(0);
	}
	if (arg->bin_path
		&& execve(arg->bin_path, args_split, shell->envp) == -1)
	{
		ft_split_del(args_split);
		if (is_directory(arg->bin_path))
			ft_error("minishell: %s: is a directory\n", 126, arg->cmd);
		else if (!has_access(arg->bin_path))
			ft_error("minishell: %s: Permission denied\n", 126, arg->cmd);
		else if (ft_strncmp(arg->bin_path, "./", 2) == 0
			|| ft_strncmp(arg->bin_path, "/", 1) == 0)
			ft_error("minishell: %s: No such file or directory\n", 127,
				arg->cmd);
		else
			ft_error("minishell: %s: command not found\n", 127, arg->cmd);
	}
}

char	*builtin_bin_path(t_shell *shell, char *builtin)
{
	char	**folders;
	int		i;
	char	*path;
	char	*path_env;

	if (file_exists(builtin))
		return (ft_strdup(builtin));
	path_env = ft_getenv(shell, "PATH");
	folders = ft_split(path_env, ':');
	i = 0;
	while (folders[i] != NULL)
	{
		path = ft_strjoin_free(ft_strdup(folders[i]), ft_strjoin("/", builtin));
		if (file_exists(path))
		{
			ft_split_del(folders);
			return (path);
		}
		free(path);
		i++;
	}
	ft_split_del(folders);
	return (NULL);
}
