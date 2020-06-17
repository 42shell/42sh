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

static int	launch_job_bg(t_job *job)
{
	t_process	*process;
	int			fd_in;

	fd_in = STDIN_FILENO;
	g_last_exit_st = 0;
	if (!g_job_control_enabled)
		fd_in = open("/dev/null", O_RDONLY);
	process = process_new(job->command, fd_in, STDOUT_FILENO);
	launch_process(process, 0);
	if (g_job_control_enabled)
	{
		put_job_bg(job, false);
		ft_printf("[%d] %d\n", job->id + 1, job->pgid);
	}
	else
		process->done = true;
	if (fd_in != STDIN_FILENO)
		close(fd_in);
	return (0);
}

int			launch_job(t_job *job)
{
	add_job(job);
	if (job->bg)
		return (launch_job_bg(job));
	eval_command(job->command);
	if (g_job_control_enabled)
		put_job_fg(job, false);
	else
		wait_for_job(job);
	return (0);
}
