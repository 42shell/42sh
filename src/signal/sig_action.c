/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_action.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/25 17:31:24 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	sig_action(t_sh *shell, int sig)
{
	static t_sh	*sh;

	if (shell)
	{
		sh = shell;
		return ;
	}
	if (sig == SIGINT || sig == SIGTSTP || sig == SIGCONT)
		;
	else if (sig == SIGWINCH)
	{
		signal(sig, SIG_DFL);
		getwinsize(&sh->term);
		getcpos(&sh->term);
		signal(sig, sig_handle);
	}
	else if ((1 <= sig && sig <= 15) || sig == 17
			|| (21 <= sig && sig <= 22) || (24 <= sig && sig <= 27))
	{
		del(sh);
		signal(sig, SIG_DFL);
		kill(0, sig);
	}
	return ;
}
