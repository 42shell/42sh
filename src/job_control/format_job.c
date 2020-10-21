/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_job.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 17:55:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void		format_exit_status(t_dstr *buf, t_process *process)
{
	char	itoa_buf[12];

	if (WIFSIGNALED(process->status))
	{
		ft_dstr_cat(buf, " Terminated (");
		ft_dstr_cat(buf, (char *)sys_signame[process->signaled]);
		ft_dstr_add(buf, ')');
	}
	else
	{
		ft_dstr_cat(buf, " Done ");
		if (WIFEXITED(process->status) && WEXITSTATUS(process->status) != 0)
		{
			ft_itoa(WEXITSTATUS(process->status), itoa_buf);
			ft_dstr_cat(buf, "(Exit ");
			ft_dstr_cat(buf, itoa_buf);
			ft_dstr_add(buf, ')');
		}
	}
}

void		format_process_info(t_dstr *buf, t_process *process, int padding)
{
	char	itoa_buf[12];

	while (padding--)
		ft_dstr_add(buf, ' ');
	if (SHOW_PID)
	{
		ft_itoa(process->pid, itoa_buf);
		ft_dstr_cat(buf, itoa_buf);
	}
	if (process->done)
		format_exit_status(buf, process);
	else if (process->stopped)
	{
		ft_dstr_cat(buf, " Stopped (");
		ft_dstr_cat(buf, (char *)sys_signame[process->signaled]);
		ft_dstr_add(buf, ')');
	}
	else
		ft_dstr_cat(buf, " Running");
}

void		format_job_id(t_dstr *buf, t_job *job)
{
	char	itoa_buf[12];

	ft_dstr_add(buf, '[');
	ft_itoa(job->id, itoa_buf);
	ft_dstr_cat(buf, itoa_buf);
	ft_dstr_add(buf, ']');
	if (is_last_job(job))
		ft_dstr_cat(buf, "+ ");
	else if (is_before_last_job(job))
		ft_dstr_cat(buf, "- ");
	else
		ft_dstr_cat(buf, "  ");
}

void		format_job_status(t_dstr *buf, t_job *job)
{
	if (job_is_done(job))
		format_exit_status(buf, job->processes);
	else if (job_is_stopped(job))
		ft_dstr_cat(buf, " Stopped");
	else
		ft_dstr_cat(buf, " Running");
}
