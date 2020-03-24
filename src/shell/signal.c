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
		;
	}
	return ;
}

void	init_sig(void)
{
	int		sig;

	sig = 0;
	while (sig++ < 32)
	{
		if (sig == SIGKILL || sig == SIGSTOP || sig == SIGSEGV)
			continue ;
	}
}
