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

bool		g_bg;

static int	launch_job_bg(t_job *job)
{
	t_process	*process;
	int			fd_in;
	int			old_bg;

	old_bg = g_bg;
	g_bg = true;
	fd_in = STDIN_FILENO;
	if (!g_job_control_enabled)
		fd_in = open("/dev/null", O_RDONLY);
	if (job->command->type == PIPELINE)
		eval_pipeline(job->command); // eval_pipeline
	else
	{
		process = process_new(job->command, fd_in, STDOUT_FILENO);
		launch_process(process, 0);
		process->stdin = 0;
	}
	if (fd_in != STDIN_FILENO)
		close(fd_in);
	put_job_bg(job, false);
	g_last_exit_st = 0;
	g_bg = old_bg;
	return (0);
}

int			launch_job(t_job *job)
{
	add_job_to_list(&g_current_jobs, job, false);
	if (job->bg)
	{
		launch_job_bg(job);
		move_job_in_persistent_list(job);
		if (g_job_control_enabled && g_shell.interactive_mode)
			ft_dprintf(2, "[%d] %d\n", job->id, job->pgid);
		return (g_last_exit_st);
	}
	eval_command(job->command);
	if (g_job_control_enabled)
	{
		put_job_fg(job, false);
		if (job_is_stopped(job))
		{
			move_job_in_persistent_list(job);
			return (g_last_exit_st);
		}
	}
	else
		wait_for_job(job);
	del_job_from_list(&g_current_jobs, job);
	return (g_last_exit_st);
}
