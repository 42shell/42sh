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

void	sigint_handler(int sig)
{
	(void)sig;
	g_interrupt = true;
}

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

void	sigchld_handler(int sig)
{
	pid_t	pid;
	int		status;

	(void)sig;
	while ((pid = waitpid(WAIT_ANY, &status, WNOHANG | WUNTRACED)) > 0)
	{
		if (set_process_status(pid, status) < 0)
		{
			ft_dprintf(2, "42sh: process %d not found.\n", pid);
			break ;
		}
	}
}

void	init_sig(void)
{
	struct sigaction	sigact;

	sigaction(SIGCHLD, NULL, &sigact);
	sigact.sa_handler = &sigchld_handler;
	sigact.sa_flags |= SA_NOCLDWAIT;
	sigaction(SIGCHLD, &sigact, NULL);
	signal(SIGINT, &sigint_handler);
	signal(SIGHUP, &sighup_handler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}
