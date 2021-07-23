#include "../../includes/minishell.h"

static void	splitoo(t_alist *l, t_args *c, char **tok, int *i)
{
	if (*i > 1 && l->prev
		&& l->prev->content->type != FLAG && !c->spaced)
	{
		tok[*i - 1] = ft_strjoin_free(tok[*i - 1], ft_strdup(c->cmd));
		(*i)--;
	}
	else
		tok[*i] = ft_strdup(c->cmd);
}

char	**ft_safesplit(t_shell *shell, t_alist *l)
{
	int		size;
	int		i;
	char	**tok;
	t_args	*c;

	size = ft_alstsize(l);
	tok = ft_calloc(size + 1, sizeof(char *));
	i = 0;
	while (l)
	{
		c = l->content;
		if (c && !(c->type == R_IN || c->type == R_OUT || c->type == R_AOUT))
		{
			if (!c->is_literal && !ft_strncmp(c->cmd, "$?", 2))
				tok[i] = ft_itoa(shell->status);
			else
				splitoo(l, c, tok, &i);
			i++;
		}
		l = l->next;
	}
	return (tok);
}

t_bool	validate_str(t_shell *shell, char *cmd)
{
	int		i;
	t_bool	status;

	i = 0;
	status = true;
	while (i < (int)ft_strlen(cmd))
	{
		if (!valid_redirects(shell, cmd, &i)
			|| !valid_quotes(shell, cmd, &i)
			|| !valid_commas(shell, cmd))
		{
			status = false;
			break ;
		}
		i++;
	}
	return (status);
}
