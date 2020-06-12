/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_job.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/06 19:32:52 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int				launch_job_in_subshell(t_job *job)
{
	t_process	*process;
	int			fd;

	job->pgid = g_shell.jobs->pgid;
	add_job(job);
	if (job->bg)
	{
		g_last_exit_st = 0;
		fd = open("/dev/null", O_RDONLY);
		//dup2_and_backup(STDIN_FILENO, fd, true);
		process = process_new(job->command, fd, STDOUT_FILENO);//if (!already_forked)
		launch_process(process, 0);
		close(fd);
		//restore_fds();
	}
	else
	{
		eval_command(job->command);
		wait_for_job(job);
	}
	return (0);
}

int				launch_job(t_job *job)
{
	t_process	*process;

	add_job(job);
	if (job->bg)
	{
		g_last_exit_st = 0;
		process = process_new(job->command, STDIN_FILENO, STDOUT_FILENO);
		launch_process(process, 0);
		if (g_job_control_enabled)
		{
			put_job_bg(job, false);
			ft_printf("[%d] %d\n", job->id + 1, job->pgid);
		}
	}
	else
	{
		eval_command(job->command);
		if (g_job_control_enabled)
			put_job_fg(job, false);
		else
			wait_for_job(job);
	}
	return (0);
}
