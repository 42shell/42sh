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
	if (job->command->type == CONNECTION
	&& job->command->value.connection->connector == PIPE)
		eval_command(job->command);
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
	add_job_to_list(&g_current_jobs, job);
	if (job->bg)
	{
		launch_job_bg(job);
		remove_command_from_list(job->command);
		add_job_to_list(&g_jobs, job_dup(job));
		if (g_job_control_enabled && g_shell.interactive_mode)
			ft_dprintf(2, "[%d] %d\n", job->id + 1, job->pgid);
	}
	else
	{
		eval_command(job->command);
		if (g_job_control_enabled)
			put_job_fg(job, false);
		else
			wait_for_job(job);
	}
	remove_job_from_list(&g_current_jobs, job->id);
	process_list_del(&job->processes);
	free(job);
	return (0);
}
