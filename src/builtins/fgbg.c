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

t_job	*get_job_by_str(char *str)
{
	t_job	*job;
	int		id;

	if (str[0] == '%')
	{
		if (str[1] == '+')
			return (g_jobs);
		if (str[1] == '-')
		{
			if (g_jobs && g_jobs->next)
				return (g_jobs->next);
			return (NULL);
		}
		id = ft_atoi(++str);
	}
	else
		id = ft_atoi(str);
	job = g_jobs;
	while (job && job->id != id)
		job = job->next;
	return (job);
}

int		builtin_bg(char **argv, __attribute__((unused)) t_array *env)
{
	t_job	*job;

	if (!g_job_control_enabled)
		return (2);
	update_jobs(false, false);
	if ((!(argv[1]) && !(job = g_jobs))
	|| (argv[1] && !(job = get_job_by_str(argv[1]))))
	{
		ft_dprintf(2, "42sh: bg: %s: No such job\n",
		argv[1] ? argv[1] : "current");
		return (2);
	}
	else if (job_is_done(job))
	{
		ft_dprintf(2, "42sh: bg: job [%d] has terminated\n", job->id);
		return (1);
	}
	continue_job(job, true);
	print_job(job, false);
	return (0);
}

int		builtin_fg(char **argv, __attribute__((unused)) t_array *env)
{
	t_job	*job;
	t_dstr	*buf;

	if (!g_job_control_enabled)
		return (2);
	update_jobs(false, false);
	if ((!(argv[1]) && !(job = g_jobs))
	|| (argv[1] && !(job = get_job_by_str(argv[1]))))
	{
		ft_dprintf(2, "42sh: fg: %s: No such job\n",
		argv[1] ? argv[1] : "current");
		return (2);
	}
	else if (job_is_done(job))
	{
		ft_dprintf(2, "42sh: fg: job [%d] has terminated\n", job->id);
		return (1);
	}
	buf = ft_dstr_new(128);
	format_processes(buf, job->processes, false);
	ft_dprintf(2, "%s\n", buf->str);
	ft_dstr_del(&buf);
	continue_job(job, false);
	return (0);
}
