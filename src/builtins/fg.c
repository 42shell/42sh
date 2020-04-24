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

int		builtin_fg(char **argv)
{
	int		id;
	t_job	*job;
	t_dstr	*format;

	if (!argv[1])
		job = g_shell.curr_job;
	else
	{
		if (argv[1][0] == '%')
			job = get_job_percent_format(argv[1]);
		else
		{
			job = g_shell.jobs;
			id = ft_atoi(argv[1]);
			while (job && job->id != id)
				job = job->next;
		}
	}
	if (!job)
	{
		ft_dprintf(2, "42sh: fg: %s: No such job\n",
								argv[1] ? argv[1] : "current");
		return (1);
	}
	format = format_job(job->command, NULL);
	ft_printf("%s\n", format->str);
	put_job_fg(job, true);
	return (0);
}
