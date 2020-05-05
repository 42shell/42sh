/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 13:50:00 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/11 13:43:37 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static t_job	*get_job_by_percent_format(char *format)
{
	t_job	*job;
	int		id;

	if (format[1] == '+')
		return (g_shell.curr_job);
	else if (format[1] == '-')
		return (g_shell.prev_job);
	id = ft_atoi(++format) - 1;
	job = g_shell.jobs;
	while (job && job->id != id)
		job = job->next;
	return (job);
}

static t_job	*get_job_ptr(char **argv)
{
	t_job	*job;
	int		id;

	if (!argv[1])
		job = g_shell.curr_job;
	else
	{
		if (argv[1][0] == '%')
			job = get_job_by_percent_format(argv[1]);
		else
		{
			job = g_shell.jobs;
			id = ft_atoi(argv[1]) - 1;
			while (job && job->id != id)
				job = job->next;
		}
	}
	return (job);
}

int				builtin_bg(char **argv)
{
	t_job	*job;
	t_dstr	*buf;

	buf = ft_dstr_new(32);
	if (!g_shell.jobs->next || !(job = get_job_ptr(argv)))
	{
		ft_dprintf(2, "42sh: bg: %s: No such job\n",
								argv[1] ? argv[1] : "current");
		return (1);
	}
	format_command(job->command, buf);
	ft_printf("%s &\n", buf->str);
	continue_job(job, true);
	ft_dstr_del(&buf);
	return (0);
}

int				builtin_fg(char **argv)
{
	t_job	*job;
	t_dstr	*buf;

	buf = ft_dstr_new(32);
	if (!g_shell.jobs->next || !(job = get_job_ptr(argv)))
	{
		ft_dprintf(2, "42sh: fg: %s: No such job\n",
								argv[1] ? argv[1] : "current");
		return (1);
	}
	format_command(job->command, buf);
	ft_printf("%s\n", buf->str);
	continue_job(job, false);
	ft_dstr_del(&buf);
	return (0);
}

int				builtin_jobs(char **argv)
{
	t_job		*job;

	update_status();
	job = g_shell.jobs->next;
	while (job)
	{
		print_job_long(job);
		job->notified = true;
		job = job->next;
	}
	return (0);
}