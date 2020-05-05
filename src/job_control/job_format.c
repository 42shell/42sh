/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_format.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/10 14:45:52 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char			*get_signaled_exit_format(int status)
{
	char			*sig;
	char			*ret;

	sig = ft_itoa(WTERMSIG(status));
	ret = ft_strjoin(" Terminated by signal ", sig);
	free(sig);
	return (ret);
}

static char			*get_signaled_stop_format(int status)
{
	char			*sig;
	char			*ret;

	sig = ft_itoa(WSTOPSIG(status));
	ret = ft_strjoin(" Stopped by signal ", sig);
	free(sig);
	return (ret);
}

char				*get_process_format(t_process *process)
{
	char			*status;
	char			*pid;
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
	pid = ft_itoa(process->pid);
	ret = ft_strjoin(pid, status);
	free(pid);
	free(status);
	return (ret);
}

char				*get_job_format(t_job *job)
{
	char			*status;
	char			*pgid;
	char			*ret;

	if (job_is_done(job))
		status = " Done";
	else if (job_is_stopped(job))
		status = " Stopped";
	else
		status = " Running";
	pgid = ft_itoa(job->pgid);
	ret = ft_strjoin(pgid, status);
	free(pgid);
	return (ret);
}