/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_format.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 17:55:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char			*get_signaled_exit_format(int status)
{
	char			sig[10];

	ft_itoa(WTERMSIG(status), sig);
	return (ft_strjoin(" Terminated by signal ", sig));
}

static char			*get_signaled_stop_format(int status)
{
	char			sig[10];

	ft_itoa(WSTOPSIG(status), sig);
	return (ft_strjoin(" Stopped by signal ", sig));
}

char				*get_process_format(t_process *process)
{
	char			*status;
	char			pid[10];
	char			*ret;

	if (process->done)
	{
		if (WIFSIGNALED(process->status))
			status = get_signaled_exit_format(process->status);
		else
			status = ft_strdup(" Terminated");
	}
	else if (WIFSTOPPED(process->status))
		status = get_signaled_stop_format(process->status);
	else
		status = ft_strdup(" Running");
	ft_itoa(process->pid, pid);
	ret = ft_strjoin(pid, status);
	free(status);
	return (ret);
}

char				*get_job_format(t_job *job)
{
	char			*status;
	char			pgid[10];
	char			*ret;

	if (job_is_done(job))
		status = " Done";
	else if (job_is_stopped(job))
		status = " Stopped";
	else
		status = " Running";
	ft_itoa(job->pgid, pgid);
	ret = ft_strjoin(pgid, status);
	return (ret);
}
