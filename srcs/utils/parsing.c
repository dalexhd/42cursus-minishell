#include "../../includes/minishell.h"

void	parse_dollar(t_shell *shell, char *cmd, size_t *i, char *line)
{
	char	*env;
	char	*tmp;

	tmp = ft_strnew(1);
	while (cmd[*i] && !ft_isspace(cmd[*i]))
	{
		if (cmd[*i] != '$')
			ft_strncat(tmp, &cmd[*i], 1);
		(*i)++;
	}
	(*i)--;
	env = ft_getenv(shell, tmp);
	if (env)
		ft_strcat(line, env);
}

char	**ft_safesplit(t_alist *list)
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
	char *dst = malloc(len);            // Allocate space
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


char	*fix_cmd(char *cmd)
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
				ft_error("Eroor to many redirects!\n", 1);
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

void	parse_tilde(t_shell *shell, char *cmd, size_t *i, char *line)
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

char	*clean_str(t_shell *shell, char *cmd)
{
	size_t	i;
	char	*line;
	t_bool	test;

	line = ft_strnew(ft_strlen(cmd));
	test = true;
	i = 0;
	while (cmd[i])
	{
		if (i > 0 && cmd[i - 1] == '\\')
			test = false;
		if (cmd[i] == '\\' && cmd[i + 1] != '\\') // If current char is '\\' and next char isn´t '\\'
			ft_strncat(line, &cmd[i], 1);
		else if (cmd[i] == '$' && test)
			parse_dollar(shell, cmd, &i, line);
		else if (cmd[i] == '~' && test)
			parse_tilde(shell, cmd, &i, line);
		else
			ft_strncat(line, &cmd[i], 1);
		i++;
	}
	return (line);
}
