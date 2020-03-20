/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 08:48:18 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/14 17:31:49 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	print_jobs(t_job *jobs)
{
	t_job		*job;
	t_pipeline	*pipe;
	t_process	*process;
	t_token		*word;
	t_redir		*redir;

	job = jobs;
	while (job)
	{
		pipe = job->pipelines;
		while (pipe)
		{
			process = pipe->processes;
			while (process)
			{
				word = process->words;
				redir = process->redirs;
				while (word)
				{
					printf("%s ", word->value->str);
					word = word->next;
				}
				while (redir)
				{
					if (redir->left_op)
						printf("%s", redir->left_op->value->str);
					printf("%s", redir->operator->value->str);
					printf("%s ", redir->right_op->value->str);
					redir = redir->next;
				}
				if ((process = process->next))
					printf("| ");
				
			}
			if (pipe->next)
				printf("%s ", pipe->sep->value->str);
			pipe = pipe->next;
				
		}
		printf("%s\n", job->sep ? job->sep->value->str : "");
		job = job->next;
	}
}