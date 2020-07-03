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

<<<<<<< HEAD
bool		g_bg;
=======
static void	make_processes_strings(t_process *list)
{
	t_process	*process;
	t_dstr		*buf;

	process = list;
	while (process)
	{
		buf = ft_dstr_new(64);
		if (process->stdin != 0)
			ft_dstr_append(buf, "| ");
		format_command(buf, process->command);
		process->command_str = buf;
		process->command = NULL;
		process = process->next;
	}
}

static void	move_job_in_persistent_list(t_job *job)
{
	make_processes_strings(job->processes);
	remove_job_from_list(&g_current_jobs, job);
	add_job_to_list(&g_jobs, job, true);
	job->command = NULL;
}
>>>>>>> Norme, attempt №1

static int	launch_job_bg(t_job *job)
{
	t_process	*process;
	t_list_head	*fd_backup;
	int			old_bg;
	int			stdin;

	old_bg = g_bg;
	g_bg = true;
	fd_backup = NULL;
	if (!g_job_control_enabled && (stdin = open("/dev/null", O_RDONLY)))
	{
		dup2_and_backup(&fd_backup, stdin, STDIN_FILENO);
		close(stdin);
	}
	if (job->command->type == PIPELINE)
		eval_pipeline(job->command);
	else
	{
		process = process_new(job->command, STDIN_FILENO, STDOUT_FILENO);
		launch_process(process, 0);
	}
	put_job_bg(job, false);
	restore_fds(&fd_backup);
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
