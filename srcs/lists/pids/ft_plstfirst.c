#include "../../../includes/minishell.h"

t_pids	*ft_plstfirst(t_pids *hst)
{
	if (!hst)
		return (0);
	while (hst->prev != 0)
	{
		hst = hst->prev;
	}
	return (hst);
}
