/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs_fgbg_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 13:50:00 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/11 13:43:37 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
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
			{
				return (g_curr_job->next->next != g_curr_job
				? g_curr_job->next->next->data : g_curr_job->next->data);
			}
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
*/
