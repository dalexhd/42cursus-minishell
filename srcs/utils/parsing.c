#include "../../includes/minishell.h"

t_bool	ft_isenv(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9') || (c == '_'))
	{
		return (true);
	}
	return (false);
}

void	parse_dollar(t_shell *shell, char **cmd, int *i, char *line)
{
	char	*env;
	char	*tmp;
	char	*aux;

	tmp = ft_strnew(1);
	if (ft_isdigit(ft_strdup(*cmd)[*i + 1]))
	{
		*(cmd) += 2;
		(*i)--;
		return ;
	}
	aux = ft_strdup(*cmd);
	(*i)++;
	while (aux[*i] && ft_isenv(aux[*i]))
	{
		if (aux[*i] != '$')
			ft_strncat(tmp, &aux[*i], 1);
		(*i)++;
	}
	(*i)--;
	env = ft_getenv(shell, tmp);
	if (env)
		ft_strcat(line, env);
}

char	**ft_safesplit(t_shell *shell, t_alist *list)
{
	int		size;
	int		i;
	char	**tokens;

	size = ft_alstsize(list);
	tokens = (char **)malloc(sizeof(char *) * (size + 1));
	i = 0;
	while (list)
	{
		if (list->content)
		{
			if (!list->content->is_literal && !ft_strcmp(list->content->cmd, "$?"))
				tokens[i] = ft_itoa(shell->exit_status);
			else
				tokens[i] = list->content->cmd;
			i++;
		}
		list = list->next;
	}
	tokens[i] = NULL;
	return (tokens);
}

char	*addLetter(char s[], char c, size_t pos)
{
	size_t i = 0;

	while (i < pos && s[i]) ++i;

	if (i == pos)
	{
		do
		{
			char tmp = s[i];
			s[i++] = c;
			c = tmp;
		} while (c);
		s[i] = c;
	}

	return s;
}

static	char *ft_strdupcus(const char *src, size_t len) {
	len = len + 1;       // String plus '\0'
	char *dst = (char *) malloc(sizeof(char) * len);            // Allocate space
	if (dst == NULL) return NULL;       // No memory
	ft_memcpy(dst, src, len);             // Copy the block
	return dst;                         // Return the new string
}


char *add_spaces(char *need_to_add, int count, int *positions)
{
	char *with_spaces;

	with_spaces =  ft_strdupcus(need_to_add, ft_strlen(need_to_add) + count);
	for (int d = 0 ; d < count; d++)
		addLetter(with_spaces, ' ', positions[d]);
	return (with_spaces);
}


char	*fix_cmd(t_shell *shell, char *cmd)
{
	int	*positions;
	int	count;
	int	d;
	int	safequotes;

	safequotes = 1;
	count = 0;
	for (size_t i = 0 ; i < ft_strlen(cmd); i++)
	{
		if (cmd[i] == '"')
			safequotes = !safequotes; // Here we set safe quotes to the opposite value
		if (safequotes && cmd[i] == '>' && cmd[i + 1] == '>')
		{
			if (cmd[i - 1] != ' ')
			count++;
			if (cmd[i + 2] != ' ')
			count++;
			i++;
		}
		else if (safequotes && (cmd[i] == '>' || cmd[i] == '<'))
		{
			if (cmd[i - 1] != ' ')
				count++;
			if (cmd[i + 1] != ' ')
				count++;
		}
	}
	positions = (int *)malloc(sizeof(int) * count);
	d = 0;
	for (size_t i = 0 ; i < ft_strlen(cmd); i++)
	{
		if (cmd[i] == '"')
			safequotes = !safequotes;
		if (safequotes && (cmd[i] == '>' || cmd[i] == '<'))
		{
			if (cmd[i] == '>' && cmd[i + 1] == '>' && cmd[i + 2] == '>')
			{
				if (shell->is_cmd)
					ft_error("minishell: -c: syntax error near unexpected token `>'\nminishell: -c: `%s'\n", 0, cmd);
				else
					ft_error("minishell: syntax error near unexpected token `>'\n", 0);
				shell->exit_status = 2;
			}
			else if (cmd[i] == '<' && cmd[i + 1] == '<' && cmd[i + 2] == '<')
			{
				if (shell->is_cmd)
					ft_error("minishell: -c: syntax error near unexpected token `<'\nminishell: -c: `%s'\n", 0, cmd);
				else
					ft_error("minishell: syntax error near unexpected token `<'\n", 0);
				shell->exit_status = 2;
			}
			if (cmd[i] == '>' && cmd[i + 1] == '>')
			{
				positions[d] = i + d;
				d++;
				positions[d] = i + d + 2;
				d++;
			}
			else
			{
				if (cmd[i - 1] != '>'  && cmd[i - 1] != '<' && cmd[i - 1] != ' ')
				{
					positions[d] = i + d;
					d++;
				}
				if (cmd[i - 1] != '>'  && cmd[i - 1] != '<' && cmd[i + 1] != ' ')
				{
					positions[d] = i + d + 1;
					d++;
				}
			}
		}
	}
	return (add_spaces(cmd, count, positions));
}

void	parse_tilde(t_shell *shell, char *cmd, int *i, char *line)
{
	char	*env;
	char	*tmp;

	tmp = ft_strnew(1);
	env = ft_getenv(shell, "HOME");
	if (env)
		ft_strcat(tmp, env);
	while (cmd[*i] && !ft_isspace(cmd[*i]))
	{
		if (cmd[*i] != '~')
			ft_strncat(tmp, &cmd[*i], 1);
		(*i)++;
	}
	(*i)--;
	ft_strcat(line, tmp);
}

char	*clean_str(t_shell *shell, t_args *arg, char *cmd)
{
	int	i;
	char	*res;
	t_bool	test;

	res = ft_strnew(ft_strlen(cmd));
	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\\' && ft_strchr("\\$\"n", cmd[i + 1]))
		{
			ft_strncat(res, &cmd[i + 1], 1);
			i++;
		}
		else if (!arg->is_literal && cmd[i] == '$' && (ft_isalpha(cmd[i + 1]) || cmd[i + 1] == '_') && test)
			parse_dollar(shell, &cmd, &i, res);
		else if (cmd[i] == '~' && test)
			parse_tilde(shell, cmd, &i, res);
		else
			ft_strncat(res, &cmd[i], 1);
		i++;
	}
	return (res);
}
