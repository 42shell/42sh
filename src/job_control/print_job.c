/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_job.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 16:46:58 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

bool		is_last_job(t_job *job)
{
	if (job && g_jobs == job)
		return (true);
	return (false);
}

bool		is_before_last_job(t_job *job)
{
	if (job && g_jobs && g_jobs->next == job)
		return (true);
	return (false);
}

/*
void		print_job_long(t_job *job)
{
	t_dstr		*command_format;
	char		*process_format;
	t_process	*process;

	if (!job)
		return ;
	print_job(job, false);
	command_format = ft_dstr_new(64);
	process = job->processes;
	while (process)
	{
		process_format = get_process_format(process);
		if (process->stdin != 0)
			ft_dstr_append(command_format, "| ");
		format_command(command_format, process->command);
		ft_dprintf(2, "     %-30s %s%s\n", process_format, command_format->str,
		(!process->next && job->bg) ? " &" : "");
		ft_dstr_clear(command_format, 64);
		free(process_format);
		process = process->next;
	}
	ft_dstr_del(&command_format);
}
*/
/*
still eval the tree to print the command,
sort jobs numerically
*/

void		print_job(t_job *job, bool print_command)
{
	t_dstr		*command_format;
	t_process	*process;
	char		*job_format;
	char		*curr;

	if (!job)
		return ;
	curr = " ";
	if (is_last_job(job))
		curr = "+";
	else if (is_before_last_job(job))
		curr = "-";
	else
		curr = " ";
	job_format = get_job_format(job);
	process = job->processes;
	command_format = ft_dstr_new(64);
	while (process)
	{
		ft_dstr_append(command_format, process->command_str->str);//printf //dstr_cat
		if (process->next)
			ft_dstr_append(command_format, " ");
		process = process->next;
	}
	if (job->bg) // and job_is_running
		ft_dstr_append(command_format, " &");
	ft_dprintf(2, "[%d]%s %-30s %s\n", job->id, curr, job_format,
	print_command ? command_format->str : "");
	if (print_command)
		ft_dstr_del(&command_format);
	free(job_format);
}
