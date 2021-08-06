#include "../../includes/minishell.h"

static char	*ft_cd_checker(t_shell *shell, char **dir, char **args)
{
	char	*pwd;

	pwd = ft_pwd();
	if (!args[1])
	{
		*dir = shell->home_dir;
		if (!*dir)
		{
			sh_error(shell, "minishell: cd: HOME not set\n", 1);
			return (NULL);
		}
		return (pwd);
	}
	*dir = args[1];
	return (pwd);
}

void	ft_cd_internal(t_shell *shell, char *dir, t_bool set_home)
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
		sh_error(shell, "minishell: cd: %s: %s\n", 1, dir, strerror(errno));
		return ;
	}
	ft_export_internal(shell, "OLDPWD", pwd);
	ft_strdel(&pwd);
	pwd = ft_pwd();
	ft_export_internal(shell, "PWD", pwd);
	ft_strdel(&pwd);
	if (set_home)
		shell->home_dir = ft_pwd();
}

void	ft_cd(t_shell *shell, char **args)
{
	char	*pwd;
	char	*dir;

	if (args[1] && args[2])
	{
		sh_error(shell, "minishell: cd: too many arguments\n", 1);
		return ;
	}
	pwd = ft_cd_checker(shell, &dir, args);
	if (!pwd)
		return ;
	if (ft_strlen(dir) > 0 && chdir(dir) == -1)
	{
		ft_strdel(&pwd);
		sh_error(shell, "minishell: cd: %s: %s\n", 1, dir, strerror(errno));
		return ;
	}
	ft_export_internal(shell, "OLDPWD", pwd);
	ft_strdel(&pwd);
	pwd = ft_pwd();
	ft_export_internal(shell, "PWD", pwd);
	ft_strdel(&pwd);
	shell->status = 0;
}
