#include "../includes/minishell.h"

void	ft_cd(t_shell *shell, char **args)
{
	char	*pwd;
	char	*dir;

	pwd = ft_pwd();
	if (!args[1])
		dir = ft_getenv(shell, "HOME");
	else
		dir = args[1];
	if (chdir(dir) == -1)
	{
		ft_strdel(&pwd);
		ft_error("minishell: cd: %s: %s\n", 0, dir, strerror(errno));
		return ;
	}
	ft_export_internal(shell, "OLDPWD", pwd);
	ft_strdel(&pwd);
	pwd = ft_pwd();
	ft_export_internal(shell, "PWD", pwd);
	ft_strdel(&pwd);
}
