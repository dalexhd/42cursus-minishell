#include "../../includes/minishell.h"

t_bool	has_redirect(t_rlist *redirect)
{
	return (redirect && (redirect->content->in.status
			|| redirect->content->out.status
			|| redirect->content->aout.status));
}
