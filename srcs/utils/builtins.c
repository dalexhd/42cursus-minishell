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
		ft_env(shell, args);
	else if (!ft_strcmp(parsed->content->args->content->cmd, "pwd"))
	{
		pwd = ft_pwd();
		ft_printf("%s\n", pwd);
		ft_strdel(&pwd);
		shell->status = 0;
	}
	else if (!ft_strcmp(parsed->content->args->content->cmd, "echo"))
		ft_echo(shell, args);
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
	t_args	*a;

	a = args->content;
	args_split = ft_safesplit(shell, args);
	if (!args_split[0])
	{
		ft_split_del(args_split);
		exit(0);
	}
	if (a->bin_path && execve(a->bin_path, args_split, shell->envp) == -1)
	{
		ft_split_del(args_split);
		if (ft_strcmp(a->bin_path, ".") && is_directory(a->bin_path))
			ft_error(ERR_I_DIR, 126, a->cmd);
		else if (!ft_strcmp(a->bin_path, "."))
			ft_error(ERR_FA""ERR_FAU, 2, a->cmd);
		else if (!has_access(a->bin_path))
			ft_error(ERR_P_DEN, 126, a->cmd);
		else if (ft_strncmp(a->bin_path, "./", 2) == 0
			|| ft_strncmp(a->bin_path, "/", 1) == 0)
			ft_error(ERR_N_FILE_DIR, 127, a->cmd);
		ft_error(ERR_C, 127, a->cmd);
	}
}

char	*builtin_bin_path_sec(t_shell *shell, char *bu)
{
	sh_error(shell, ERR_N_FILE_DIR, 127, bu);
	shell->parsed->content->valid = false;
	return (ft_strdup(bu));
}

char	*builtin_bin_path(t_shell *shell, char *bu)
{
	char	**folders;
	int		i;
	char	*path;
	char	*path_env;

	if (!file_exists(bu))
	{
		path_env = ft_getenv(shell, "PATH", false);
		if (!path_env)
			return (builtin_bin_path_sec(shell, bu));
		folders = ft_split(path_env, ':');
		i = 0;
		path = NULL;
		while (folders[++i])
		{
			path = ft_strjoin_free(ft_strdup(folders[i]), ft_strjoin("/", bu));
			if (file_exists(path))
				break ;
			ft_strdel(&path);
		}
		ft_split_del(folders);
		return (path);
	}
	else
		return (ft_strdup(bu));
}
