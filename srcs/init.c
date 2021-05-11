/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 18:21:08 by aborboll          #+#    #+#             */
/*   Updated: 2021/05/12 01:36:52 by aborboll         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \r\n\a'\\"

t_bool file_exists(char *filename)
{
	struct stat buffer;

	return (stat(filename, &buffer) == 0);
}

static char *builtin_bin_path(t_shell *shell, char *builtin)
{
	char **folders;
	int i;
	char *path;

	folders = ft_split(ft_getenv(shell, "PATH"), ':');
	i = 0;
	while (folders[i] != NULL)
	{
		path = ft_strjoin(folders[i], ft_strjoin("/", builtin));
		if (file_exists(path))
			return (path);
		i++;
	}
	return (NULL);
}

static void	exec(t_shell *shell, char **args)
{
	if (ft_isbuiltin(args[0]))
	{
		if (!ft_strcmp(args[0], "cd"))
			ft_cd(shell, args[1]);
		else if (!ft_strcmp(args[0], "env"))
			ft_env(shell);
		else if (!ft_strcmp(args[0], "pwd"))
			ft_printf("%s\n", ft_pwd());
	}
	else if (execve(builtin_bin_path(shell, args[0]), args, shell->envp) == -1)
			ft_error("permission denied", 1);
}

/* void test_extbuiltin(t_shell *shell, t_slist *parsed_lst)
{
	pid_t pid;
	int fd[2];


	if (parsed_lst->content->flags.has_stdout)
		pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		if (parsed_lst->content->flags.has_stdout)
		{
			parsed_lst->next->content->flags.has_stdin = true;
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
		}
		else if (parsed_lst->content->flags.has_stdin)
		{
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
		} 
		exec(shell, parsed_lst->content->args);
	}
	else if (pid < 0)
		ft_error("failed to fork", 1);
	else
	{
		pid = fork();
		if (pid == 0)
		{
			if (parsed_lst->content->flags.has_stdout)
			{
				dup2(fd[0], STDIN_FILENO);
				close(fd[0]); 
			}
			exec(shell, parsed_lst->next->content->args);
			if (parsed_lst->next->content->flags.has_pipe)
				test_extbuiltin(shell, parsed_lst->next);
		}
	}
} */

void test_extbuiltin(t_shell *shell, t_slist *parsed_lst, int fd[2])
{
	pid_t pid;

	if (parsed_lst->content->flags.has_stdout)
		pipe(fd);
	pid = fork();
	if (pid == 0)
	{

		if (parsed_lst->content->flags.has_stdout)
		{
			parsed_lst->next->content->flags.has_stdin = true;
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
		}
		else if (parsed_lst->content->flags.has_stdin)
		{
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
		} 
		exec(shell, parsed_lst->content->args);
	}
	else if (pid < 0)
		ft_error("failed to fork", 1);
	if (parsed_lst->next)
	{
		parsed_lst->next->content->flags.has_stdin = parsed_lst->content->flags.has_stdout;
		test_extbuiltin(shell, parsed_lst->next, fd);
	}
}

void test_intextbuiltin(t_shell *shell)
{
	pid_t pid;
	int fd[2];

	char *grepArgs[] = {"grep", "HOME", (char *)0};

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		ft_env(shell);
	}
	else if (pid < 0)
		ft_error("failed to fork", 1);
	else
	{
		pid = fork();
		if (pid == 0)
		{
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
			if (execve("/usr/bin/grep", grepArgs, shell->envp) == -1)
				ft_error("permission denied", 1);
		}
	}
}

void test_builtins(t_shell *shell)
{
	/* 	char	*array[] = {
		"hello world"
	};
	ft_echo(array, sizeof(array) / sizeof(char *)); */

	ft_printf("\n");
	ft_printf("Current directory: %s\n", ft_pwd());
	ft_printf("PWD env value=%s\n", ft_getenv(shell, "PWD"));
	ft_printf("OLDPWD env value=%s\n", ft_getenv(shell, "OLDPWD"));
	ft_info("Lets move directory ../", false);
	ft_cd(shell, "../");
	ft_printf("Current directory: %s\n", ft_pwd());
	ft_printf("PWD env value=%s\n", ft_getenv(shell, "PWD"));
	ft_printf("OLDPWD env value=%s\n", ft_getenv(shell, "OLDPWD"));
	ft_info("Lets unset HOME env variable", false);
	ft_unset(shell, "HOME");
	ft_printf("HOME env value=%s\n", ft_getenv(shell, "HOME"));
}

t_bool	has_pipe(t_parsed *parsed, char *token)
{
	if (ft_strcmp("|", token) == 0)
	{
		(*parsed).flags.has_pipe = true;
		(*parsed).flags.has_stdout = true;
		return (true);
	}
	return (false);
}

void lsh_split_line(t_shell *shell, char *line)
{
	int			bufsize = LSH_TOK_BUFSIZE, position = 0;
	char		**tokens = malloc(bufsize * sizeof(char *));
	char 		*token;
	t_parsed	*parsed;

	if (!tokens)
	{
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}
	parsed = (t_parsed *)malloc(sizeof(t_parsed));
	parsed->flags.has_pipe = false;
	parsed->line = ft_strdup(line);
	parsed->flags.has_stdin = false;
	parsed->flags.has_stdout = false;
	token = strtok(line, LSH_TOK_DELIM);
	while (token != NULL && !has_pipe(parsed, token))
	{
		tokens[position] = token;
		position++;
		if (position >= bufsize)
		{
			bufsize += LSH_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens)
			{
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, LSH_TOK_DELIM);
	}
	tokens[position] = NULL;
	parsed->args = tokens;
	ft_slstadd_back(&shell->parsed, ft_slstnew(parsed));
	if (parsed->flags.has_pipe)
		lsh_split_line(shell, ft_substr(parsed->line, token - line + 1, ft_strlen(parsed->line)));
}

t_shell *init_shell(char *cmd, char **envp)
{
	t_shell *shell;
	int		fd[2];

	shell = malloc(sizeof(t_shell));
	shell->running = true;
	shell->envp = envp;
	shell->parsed = NULL;
	lsh_split_line(shell, cmd);
	//ft_slstadd_back(&shell.parsed, ft_slstnew(lsh_split_line(ft_strdup("grep PATH"))));
	//test_builtins(shell);
	test_extbuiltin(shell, shell->parsed, fd);
	//test_intextbuiltin(shell);
	return (shell);
}

//ft_lineexec("export HOLA=adios && env | grep HOLA");