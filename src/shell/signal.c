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

void	sighup_handler(int sig)
{
	t_job	*job;

	(void)sig;
	job = g_shell.jobs;
	while (job)
	{
		if (job_is_stopped(job))
			kill(-job->pgid, SIGCONT);
		kill(-job->pgid, SIGHUP);
		job = job->next;
	}
	if (g_shell.interactive_mode)
		tcsetattr(STDIN_FILENO, TCSADRAIN, &g_shell.tmodes);
	exit(129);
}

void	init_sig(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGHUP, &sighup_handler);
}
