/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_del.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/09 18:59:30 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_process	*process_new(t_command *cmd, int stdin, int stdout)
{
	t_process	*process;

	process = (t_process *)ft_xmalloc(sizeof(t_process));
	process->stdin = stdin;
	process->stdout = stdout;
	process->command = cmd;
	return (process);
}

t_job		*job_new(t_command *cmd, int stdin, int stdout)
{
	t_job	*job;

	job = (t_job *)ft_xmalloc(sizeof(t_job));
	job->invert_ret = cmd->flags & CMD_INVERT_RETURN;
	job->stdin = stdin;
	job->stdout = stdout;
	job->command = cmd;
	return (job);
}

void		process_list_del(t_process **process_list)
{
	t_process	*next;

	while (process_list && *process_list)
	{
		next = (*process_list)->next;
		ft_dstr_del(&(*process_list)->command_str);
		free(*process_list);
		*process_list = next;
	}
}
