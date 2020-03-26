/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2019/11/22 00:37:04 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	sig_handle(int sig)
{
	if (sig == SIGWINCH)
	{
		;
	}
	else if (SIGTSTP <= sig && sig <= SIGTTOU)
	{
		signal(sig, SIG_IGN);;
	}
	return ;
}

void	init_sig(void)
{
	int		sig;

	sig = 0;
	signal(SIGCHLD, SIG_IGN);
	while (sig++ < 32)
	{
		if (SIGTSTP <= sig && sig <= SIGTTOU)
		{
			signal(sig, sig_handle);
		}
		if (sig == SIGKILL || sig == SIGSTOP || sig == SIGSEGV)
			continue ;
	}
}
