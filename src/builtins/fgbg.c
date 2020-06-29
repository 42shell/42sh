/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fgbg.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 13:50:00 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/09 15:32:38 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
fg : remove_job_from_list(g_jobs)
	 add_job_to_list(g_current_jobs)
	 put the fucking ast in g_complete_command to be deleted if job completes
	 put_job_fg()
	 	if stopped again will dup again and put it on top of the stack
	del the job like in launch_job

bg : job already in the list.
	 put_job_bg()

both cases, if job_is_done command will be deleted in notif_jobs

notif_jobs : the job was stopped while in the background,
			 put the job on top of the stack (need prev_job=no))

*/

/*
static int	notif_and_continue_job(t_job *job, bool bg)
{
	//t_dstr		*command_format;

	//command_format = ft_dstr_new(32);
	//format_command(command_format, job->command);
	//ft_printf("%s%s\n", command_format->str, bg ? " &" : "");
	//ft_dstr_del(&command_format);
	return (0);
}
*/

int			builtin_bg(char **argv, __attribute__((unused)) t_array *env)
{
	t_job	*job;

	if (!g_job_control_enabled)
		return (2);
	update_jobs(false);
	if ((!(argv[1]) && !(job = g_jobs))
	|| (argv[1] && !(job = get_job_by_str(argv[1]))))
	{
		ft_dprintf(2, "42sh: bg: %s: No such job\n",
		argv[1] ? argv[1] : "current");
		return (2);
	}
	continue_job(job, true);
	print_job(job, false);
	return (0);
}


int			builtin_fg(char **argv, __attribute__((unused)) t_array *env)
{
	t_job	*job;
	t_dstr	*buf;

	if (!g_job_control_enabled)
		return (2);
	update_jobs(false);
	if ((!(argv[1]) && !(job = g_jobs))
	|| (argv[1] && !(job = get_job_by_str(argv[1]))))
	{
		ft_dprintf(2, "42sh: fg: %s: No such job\n",
		argv[1] ? argv[1] : "current");
		return (2);
	}
	buf = ft_dstr_new(128);
	format_processes(buf, job->processes, false);
	ft_dprintf(2, "%s\n", buf->str);
	ft_dstr_del(&buf);
	continue_job(job, false);
	return (0);
}
