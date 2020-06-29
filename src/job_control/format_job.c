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

void	format_process_info(t_dstr *buf, t_process *process, int padding)
{
	char	itoa_buf[12];

	while (padding--)
		ft_dstr_cat(buf, " ");
	ft_itoa(process->pid, itoa_buf);
	ft_dstr_cat(buf, itoa_buf);
	if (process->done)
	{
		ft_dstr_cat(buf, " Terminated");
		if (WIFSIGNALED(process->status))
		{
			ft_itoa(WSTOPSIG(process->status), itoa_buf);
			ft_dstr_cat(buf, " by signal ");
			ft_dstr_cat(buf, itoa_buf);
		}
	}
	else if (process->stopped)
	{
		ft_itoa(WSTOPSIG(process->status), itoa_buf);
		ft_dstr_cat(buf, " Stopped by signal ");
		ft_dstr_cat(buf, itoa_buf);
	}
	else
		ft_dstr_cat(buf, " Running");
}

void	format_job_info(t_dstr *buf, t_job *job)
{
	char	id[12];

	ft_dstr_cat(buf, "[");
	ft_itoa(job->id, id);
	ft_dstr_cat(buf, id);
	ft_dstr_cat(buf, "]");
	if (is_last_job(job))
		ft_dstr_cat(buf, "+ ");
	else if (is_before_last_job(job))
		ft_dstr_cat(buf, "- ");
	else
		ft_dstr_cat(buf, "  ");
	ft_itoa(job->pgid, id);
	ft_dstr_cat(buf, id);
	if (job_is_done(job))
		ft_dstr_cat(buf, " Done");
	else if (job_is_stopped(job))
		ft_dstr_cat(buf, " Stopped");
	else
		ft_dstr_cat(buf, " Running");
}
