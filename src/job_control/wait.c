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
*/

static void	set_exit_status(t_process *process)
{
	if (WIFEXITED(process->status))
	{
		if (process->stdout == 1)
			g_last_exit_st = WEXITSTATUS(process->status);
	}
	else if (WIFSIGNALED(process->status))
	{
		process->signaled = WTERMSIG(process->status);
		if (process->stdout == 1)
			g_last_exit_st = process->signaled + 128;
	}
}

/*
** If the process is stopped and it is already in the persistent jobs list (it
** was running in bg or has been previously stopped), we put the associated job
** to the top of the stack.
** Otherwise (the job was in fg and it has never been stopped or put in bg),
** we move it from the current jobs to the persistent list.
** This corresponds to the '[job_id]+' in jobs builtin and allow to fg/bg the
** last stopped/bg'd job without knowing its id.
*/

int			set_process_status(pid_t pid, int status)
{
	t_process	*process;

	if (!(process = get_process(pid)))
		return (-1);
	process->status = status;
	process->job->notified = false;
	if (WIFSTOPPED(status))
	{
		process->stopped = true;
		process->signaled = WSTOPSIG(process->status);
		g_last_exit_st = 128 + process->signaled;
	}
	else if (WIFCONTINUED(status))
		process->stopped = false;
	else
	{
		process->done = true;
		process->signaled = false;
		set_exit_status(process);
	}
	return (0);
}

void		update_status(void)
{
	pid_t			pid;
	int				status;

	while ((pid = waitpid(WAIT_ANY, &status,
			WNOHANG | WUNTRACED | WCONTINUED)) > 0)
	{
		if (set_process_status(pid, status) < 0)
		{
			ft_dprintf(2, "42sh: process %d not found.\n", pid);
			break ;
		}
	}
}

void		wait_for_job(t_job *job)
{
	pid_t	pid;
	int		status;

	if (!job)
		return ;
	while (!job_is_done(job)
	&& (g_job_control_enabled ? !job_is_stopped(job) : 1))
	{
		if ((pid = waitpid(WAIT_ANY, &status, WUNTRACED)) < 0)
			break ;
		if (pid > 0 && set_process_status(pid, status) < 0)
		{
			ft_dprintf(2, "42sh: process %d not found.\n", pid);
			break ;
		}
	}
	if (job_is_done(job))
	{
		if (g_last_exit_st == 130)
			g_interrupt = true;
		if (job->invert_ret)
			g_last_exit_st = g_last_exit_st ? 0 : 1;
	}
	else if (g_job_control_enabled)
		move_job_in_persistent_list(job);
}
