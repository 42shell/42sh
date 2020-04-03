/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:52:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_process	*process_new(t_node *ast, int stdin, int stdout)
{
	t_process	*process;

	process = (t_process *)ft_xmalloc(sizeof(t_process));
	process->stdin = stdin;
	process->stdout = stdout;
	process->ast = ast;
	return (process);
}

t_job	*job_new(t_node *ast, int stdin, int stdout)
{
	t_job	*job;

	job = (t_job *)ft_xmalloc(sizeof(t_job));
	job->stdin = stdin;
	job->stdout = stdout;
	job->ast = ast;
	return (job);
}

void	process_del(t_process **process)
{
	t_process	*next;

	while (*process)
	{
		next = (*process)->next;
		free(*process);
		*process = next;
	}
}

void	job_del(t_job **job)
{
	t_job	*next;

	while (*job)
	{
		process_del(&(*job)->processes);
		ast_del(&(*job)->ast);
		next = (*job)->next;
		free(*job);
		*job = next;
	}
}