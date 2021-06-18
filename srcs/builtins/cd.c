#include "../includes/minishell.h"

static char	*ft_cd_checker(t_shell *shell, char **dir, char **args)
{
	char	*pwd;

	pwd = ft_pwd();
	if (!args[1])
	{
		*dir = ft_getenv(shell, "HOME");
		if (!*dir)
		{
			ft_error("minishell: cd: HOME not set\n", 0);
			shell->exit_status = 1;
			return (NULL);
		}
	}
	else
		*dir = args[1];
	return (pwd);
}

void	ft_cd(t_shell *shell, char **args)
{
	char	*pwd;
	char	*dir;

	pwd = ft_cd_checker(shell, &dir, args);
	if (!pwd)
		return ;
	if (chdir(dir) == -1)
	{
		ft_strdel(&pwd);
		ft_error("minishell: cd: %s: %s\n", 0, dir, strerror(errno));
		shell->exit_status = 1;
		return ;
	}
	ft_export_internal(shell, "OLDPWD", pwd);
	ft_strdel(&pwd);
	pwd = ft_pwd();
	ft_export_internal(shell, "PWD", pwd);
	ft_strdel(&pwd);
}
