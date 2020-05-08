/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 13:50:00 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/11 13:43:37 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_job		*get_job_by_str(char *str)
{
	t_job	*job;
	int		id;

	if (str[0] == '%')
	{
		if (str[1] == '+')
			return ((g_curr_job && g_curr_job->next != g_curr_job)
			? g_curr_job->next->data : NULL);
		else if (str[1] == '-')
		{
			if (g_curr_job && g_curr_job->next != g_curr_job)
				return (g_curr_job->next->next != g_curr_job
				? g_curr_job->next->next->data : g_curr_job->next->data);
			return (NULL);
		}
		id = ft_atoi(++str) - 1;
	}
	else
		id = ft_atoi(str) - 1;
	job = g_shell.jobs->next;
	while (job && job->id != id)
		job = job->next;
	return (job);
}

t_list_head	*get_fgbg_jobs_list(char **argv)
{
	t_list_head	*list;
	t_job		*job;

	list = ft_list_first_head(NULL);
	if (!*argv)
	{
		if (!g_curr_job || g_curr_job->next == g_curr_job)
			g_jobspec_error = ft_strdup(*argv);
		else
			ft_list_add(g_curr_job->next->data, list);
		return (list);
	}
	while (*argv)
	{
		if (!(job = get_job_by_str(*argv)))
		{
			g_jobspec_error = ft_strdup(*argv);
			return (list);
		}
		ft_list_add_tail(job, list);
		argv++;
	}
	return (list);
}

t_list_head	*get_jobs_jobs_list(char **argv)
{
	t_list_head	*list;
	t_job		*job;

	list = ft_list_first_head(NULL);
	if (!*argv)
	{
		job = g_shell.jobs->next;
		while (job)
		{
			ft_list_add(job, list);
			job = job->next;
		}
		return (list);
	}
	while (*argv)
	{
		if (!(job = get_job_by_str(*argv)))
		{
			g_jobspec_error = *argv;
			return (list);
		}
		ft_list_add_tail(job, list);
		argv++;
	}
	return (list);
}