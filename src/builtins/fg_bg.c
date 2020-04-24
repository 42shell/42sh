/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fg.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 13:50:00 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/11 13:43:37 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static t_job	*get_job_ptr(char **argv)
{
	t_job	*job;
	int		id;

	if (!argv[1])
		job = g_shell.curr_job;
	else
	{
		if (argv[1][0] == '%')
			job = get_job_percent_format(argv[1]);
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
	t_dstr	*format;

	if (!g_shell.jobs->next || !(job = get_job_ptr(argv)))
	{
		ft_dprintf(2, "42sh: bg: %s: No such job\n",
								argv[1] ? argv[1] : "current");
		return (1);
	}
	format = format_job(job->command, NULL);
	ft_printf("%s &\n", format->str);
	continue_job(job, true);
	return (0);
}

int				builtin_fg(char **argv)
{
	t_job	*job;
	t_dstr	*format;

	if (!g_shell.jobs->next || !(job = get_job_ptr(argv)))
	{
		ft_dprintf(2, "42sh: fg: %s: No such job\n",
								argv[1] ? argv[1] : "current");
		return (1);
	}
	format = format_job(job->command, NULL);
	ft_printf("%s\n", format->str);
	continue_job(job, false);
	return (0);
}
