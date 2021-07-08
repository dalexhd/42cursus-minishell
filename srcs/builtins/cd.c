#include "../includes/minishell.h"

static char	*ft_cd_checker(t_shell *shell, char **dir, char **args)
{
	char	*pwd;

	pwd = ft_pwd();
	if (!args[1])
	{
		*dir = shell->home_dir;
		if (!*dir)
		{
			ft_error("minishell: cd: HOME not set\n", 0);
			shell->status = 1;
			return (NULL);
		}
	}
	else
		*dir = args[1];
	return (pwd);
}

void	ft_cd_internal(t_shell *shell, char *dir)
{
	char	*pwd;

	pwd = ft_pwd();
	if (!pwd)
		return ;
	if (ft_strcmp(dir, "") == 0)
		dir = pwd;
	if (chdir(dir) == -1)
	{
		ft_strdel(&pwd);
		ft_error("minishell: cd: %s: %s\n", 0, dir, strerror(errno));
		shell->status = 1;
		return ;
	}
	ft_export_internal(shell, "OLDPWD", pwd);
	ft_strdel(&pwd);
	pwd = ft_pwd();
	ft_export_internal(shell, "PWD", pwd);
	ft_strdel(&pwd);
}

void	ft_cd(t_shell *shell, char **args)
{
	char	*pwd;
	char	*dir;

	if (args[2])
	{
		ft_error("minishell: cd: too many arguments\n", 1);
		return ;
	}
	pwd = ft_cd_checker(shell, &dir, args);
	if (!pwd)
		return ;
	if (chdir(dir) == -1)
	{
		ft_strdel(&pwd);
		ft_error("minishell: cd: %s: %s\n", 0, dir, strerror(errno));
		shell->status = 1;
		return ;
	}
	ft_export_internal(shell, "OLDPWD", pwd);
	ft_strdel(&pwd);
	pwd = ft_pwd();
	ft_export_internal(shell, "PWD", pwd);
	ft_strdel(&pwd);
	shell->status = 0;
}
