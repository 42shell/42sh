/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_current.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/11 17:07:24 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

bool		is_current_job(t_job *job)
{
	if (job && g_curr_job && g_curr_job->next != g_curr_job
	&& (t_job *)g_curr_job->next->data == job)
		return (true);
	return (false);
}

bool		is_previous_job(t_job *job)
{
	if (job && g_curr_job && g_curr_job->next->next != g_curr_job
	&& (t_job *)g_curr_job->next->next->data == job)
		return (true);
	return (false);
}

t_list_head	*get_job_current_list_elem(t_job *job)
{
	t_list_head	*curr;
	t_job		*data;

	if (!g_curr_job || g_curr_job->next == g_curr_job)
		return (NULL);
	curr = g_curr_job->next;
	while (curr != g_curr_job)
	{
		data = (t_job *)curr->data;
		if (data == job)
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

void		remove_current_list_elem(t_list_head *elem)
{
	if (elem->prev)
		elem->prev->next = elem->next;
	if (elem->next)
		elem->next->prev = elem->prev;
}

void		update_curr_job(t_job *job)
{
	t_list_head	*curr;

	if (!g_curr_job || g_curr_job->next == g_curr_job)
	{
		if (!g_curr_job)
			g_curr_job = ft_list_first_head(NULL);
		ft_list_add(job, g_curr_job);
		return ;
	}
	if ((t_job *)g_curr_job->next->data == job)
		return ;
	if ((curr = get_job_current_list_elem(job)))
	{
		remove_current_list_elem(curr);
		curr->next = g_curr_job->next;
		curr->prev = g_curr_job;
		g_curr_job->next->prev = curr;
		g_curr_job->next = curr;
		return ;
	}
	ft_list_add(job, g_curr_job);
}
