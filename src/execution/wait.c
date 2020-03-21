/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:52:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			wait_for_job(t_job *job)
{
	int		status;

	waitpid(job->pgid, &status, 0);
	g_last_exit_st = WEXITSTATUS(status);
	return (g_last_exit_st);
}

int			wait_for_pipeline(t_pipeline *pipeline)
{
	t_process	*current;
	t_process	*last;

	current = pipeline->processes;
	while (current)
	{
		last = current;
		waitpid(current->pid, &current->status, 0);
		current = current->next;
	}
	g_last_exit_st = WEXITSTATUS(last->status);
	return (g_last_exit_st);
}