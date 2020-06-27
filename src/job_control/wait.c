/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/25 02:48:20 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** We only set last_exit_st if stdout == 1 to ignore the exit status of the
** first commands of a pipeline
** TODO: only print necessary signals
*/

static int	set_process_status(pid_t pid, int status)
{
	t_process	*process;

	if (!(process = get_process(pid)))
		return (-1);
	process->status = status;
	if (WIFSTOPPED(status))
	{
		process->stopped = true;
		process->signaled = WSTOPSIG(process->status);
		g_last_exit_st = 128 + process->signaled;
	}
	else
	{
		process->done = true;
		if (WIFEXITED(status) && process->stdout == 1)
			g_last_exit_st = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			process->signaled = WTERMSIG(process->status);
			if (process->stdout == 1)
				g_last_exit_st = process->signaled + 128;
			if (process->signaled != 13 && process->signaled != 2)
				ft_dprintf(2, "%d: Terminated by signal %d.\n",
				(int)process->pid, WTERMSIG(process->status));
		}
	}
	return (0);
}

void		update_status(void)
{
	pid_t	pid;
	int		status;

	pid = 0;
	while ((pid = waitpid(WAIT_ANY, &status, WNOHANG | WUNTRACED)) > 0)
	{
		if (set_process_status(pid, status) < 0)
		{
			ft_dprintf(2, "42sh: process %d not found.\n", pid);
			break ;
		}
	}
	if (pid < 0 && errno != ECHILD)
		ft_dprintf(2, "42sh: waitpid: unexpected error.\n", pid);
}

void		wait_for_job(t_job *job)
{
	pid_t		pid;
	int			status;

	status = 0;
	if (!job->processes)
		return ;
	while (!job_is_done(job)
	&& (g_job_control_enabled ? !job_is_stopped(job) : 1))
	{
		pid = waitpid(WAIT_ANY, &status, WUNTRACED);
		if (pid > 0 && set_process_status(pid, status) < 0)
		{
			ft_dprintf(2, "42sh: process %d not found.\n", pid);
			break ;
		}
		else if (pid < 0)
		{
			ft_dprintf(2, "42sh: waitpid: unexpected error.\n", pid);
			break ;
		}
	}
	if (job_is_done(job))
	{
		if (job->command->flags & CMD_INVERT_RETURN)
			g_last_exit_st = g_last_exit_st ? 0 : 1;
	}
}
