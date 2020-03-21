/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:52:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"


t_job	*get_job(pid_t pgid)
{
	(void)pgid;
	return (NULL);
}

bool	job_is_stopped(t_job *job)
{
	(void)job;
	return (false);
}

bool	job_is_done(t_job *job)
{
	(void)job;
	return (false);
}