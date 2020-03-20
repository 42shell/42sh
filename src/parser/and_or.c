/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_or.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 18:32:06 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** and_or			: pipe_sequence AND_IF linebreak and_or
**					| pipe_sequence OR_IF linebreak and_or
**					| pipe_sequence
** 
** returns a t_job containing a list of pipelines
** the separator operator is stored in the job struct
*/

t_job			*and_or(void)
{
	t_job		*job;
	t_pipeline	*list;
	t_pipeline	*pipe_seq;

	if (g_parser.error || !(pipe_seq = pipe_sequence()))
		return (NULL);
	list = pipe_seq;
	while (g_parser.token
	&& (g_parser.token->type == AND_IF || g_parser.token->type == OR_IF))
	{
		pipe_seq->sep = g_parser.token;
		g_parser.token = get_next_token();
		linebreak(pipe_seq->sep->type);
		if (!(pipe_seq->next = pipe_sequence()))
		{
			g_parser.error = g_parser.error ? g_parser.error : NO_CMD_AFTER_PIPE;
			free_pipelines(&list);
			return (NULL);
		}
		pipe_seq = pipe_seq->next;
	}
	job = (t_job *)ft_xmalloc(sizeof(t_job));
	job->pipelines = list;
	return (job);
}
