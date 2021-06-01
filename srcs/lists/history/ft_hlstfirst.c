#include "../../../includes/minishell.h"

t_hist	*ft_hlstfirst(t_hist *hst)
{
	if (!hst)
		return (0);
	while (hst->prev != 0)
	{
		hst = hst->prev;
	}
	return (hst);
}
