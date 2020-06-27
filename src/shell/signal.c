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
	job = g_jobs;
	while (job)
	{
		kill(-job->pgid, SIGHUP);
		if (job_is_stopped(job))
			kill(-job->pgid, SIGCONT);
		job = job->next;
	}
	if (g_shell.interactive_mode)
		tcsetattr(STDIN_FILENO, TCSADRAIN, &g_shell.tmodes);
	exit(129);
}

/*
void	sigtstp_handler(int sig)
{
	t_job	*job;

	(void)sig;
	job = g_jobs;
	while (job)
	{
		if (job_is_stopped(job) && !(g_jobs == job))
		{
			remove_job_from_list(&g_jobs, job);
			add_job_to_list(&g_jobs, job, false);
		}
		job = job->next;
	}
}
*/

void	sigint_handler(int sig)
{
	(void)sig;
	g_interrupt = true;
}

void	init_sig(void)
{
	signal(SIGINT, &sigint_handler);
	signal(SIGHUP, &sighup_handler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}
